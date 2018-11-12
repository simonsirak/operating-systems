# Locks
A lock is a structure that helps give atomic behaviour for critical sections of the code (i.e code in multithreaded processes that 
contains race conditions). The main things we want in a lock is to create atomicity and do so efficiently and with fairness. Efficiency 
is usually measured in terms of a single processor with no threads contending for the lock, with several threads contending and with several 
threads contending on a multicore CPU.

The indeterminate nature of multithreading makes it difficult to implement locks without hardware support. You usually need some atomic 
hardware instruction that modifies one value and returns another in order to implement some lock mechanism. This is why a simple variable 
that checks entrance to a code section won't do (untimely context switches can break the logic of the checking code). Below are some 
examples of hardware support.

## Test-and-set
Test-and-set is a general term for a hardware instruction that takes in a pointer and a value, then atomically loads the new value 
to the address as well as return the old value at the pointed-to address. You can implement a spin-lock using TestAndSet as follows:

```C
void lock(lock_t *lock){
   while(TestAndSet(&(lock->flag), 1) == 1);
}

void unlock(lock_t *lock){
   lock->flag = 0;
}
```

This lock will spin in the while loop for as long as the current value of the flag is 1. Also, it will keep setting the value to 1. When 
someone has finished with the lock, they will unlock and reset the flag, letting any spinning threads exit the while-loop. This satisfies 
the correctness of a lock, but is neither fair (no way of scheduling threads who are waiting for lock) nor efficient (due to spinning). 
We will see ways of minimizing spinning later.

## Compare-and-swap
Compare-and-swap is a bit more general than test-and-set. This hardware instruction takes a pointer, one value for comparison and one value
for swapping. If the value at the pointed-to address is the same as the value up for comparison, then the value at the pointed-to address 
will be swapped to the value used for swapping. In either case, the old value is returned.

We can use this in a similar manner to test-and-set to create an identical lock:

```C
void lock(lock_t *lock){
   while(CompareAndSwap(&(lock->flag), 0, 1) == 1);
}
```

## Load-Linked and Store-Conditional
These are a pair of instructions meant to be used together. Used in the MIPS architecture. Essentially, load-linked says "I'm 
about to use the loaded lock, don't touch it!". If store-conditional manages to run before someone else mutates the value of the 
lock, then we store a new value into the lock flag and return 1 (success), otherwise we do nothing and return 0.

Building a lock using these requires two steps: First, check that the flag has value 0 using load-linked in a spinning matter. Upon 
success, check if store-conditional is successful; if yes, then you have acquired the lock; if no, then you have to repeat the entire 
lock procedure again.

## Fetch-and-add
This primitive takes in a pointer, increments its corresponding value by 1 and returns the old pointer. We can build a ticket 
lock using this primitive by letting each lock store the current turn and the next available ticket. Each lock-call acquires its 
turn by grabbing the next available ticket using `FetchAndAdd(lock->ticket)`. It then spins until it is the corresponding callers turn. When we unlock, we also make sure to increment the turn by 1 as well, so that the next caller can have the lock. This ensures fairness 
as well. However, it is not efficient still, since the spinning depends on the user-code that the lock encapsulates. A context switch 
while a lock is held is suboptimal if other threads try to access that same critical section -- they will simply spin.

## Yield
Instead of spinning, one could use an OS-instruction called `yield` to force a context switch if the lock has not been acquired. That 
removes the need to wait for timer interrupts for each thread waiting for the lock. However you still need to make the context switches, 
which is still inefficient. This approach can, just like every other previous approach, also not fix starvation (i.e thread never gets 
the lock) since it might just get passed around to other threads.

## Lock with Queues
We can implement locks using test-and-set and queues to achieve minimal spinning, fairness and correctness all at once! 

A lock should contain a flag value, a guard value and a queue. 

The guard value is used as "a lock for the lock-function" with the help of TestAndSet, so that no two threads can be inside the lock function at the same time. This causes spinning, but only until the other thread currently executing the lock-function can finish the lock-function. So the spinning does not depend on the user code. 

Once a particular thread is in the lock function, we check if the lock itself is occupied. If not, we acquire it and reset the guard value. If yes, then we add the thread to the lock queue, reset the guard and park the thread. The parking means put the thread to sleep until someone explicitly unparks the thread. Note that we never set the lock flag to 0 unless the queue is 
empty. This is because we otherwise "pass the baton" from the releasing thread to the thread next up for getting the lock.

A thread is unparked once the lock is unlocked by the currently "lock-owning" thread. An implementation can be found in page 318 of "Operating Systems: Three Easy Pieces".

This implementation can be the victim of wakeup races, a special kind of data race occuring when a thread is about to be parked (until the lock is released), but then a context-switch occurs to a thread that releases the lock (with no other threads waiting for the lock). Then, if the originally mentioned thread is parked, it might stay parked indefinitely. This can be solved by another OS-function (to be used before resetting the guard) that we are about to park. This function notifies 
the OS that if someone unparks the thread before it is actually parked, the park will be a no-op (no operation done).

> Questions:
> Locks are implemented using queues. Are the queues themselves concurrently implemented?
> Why is volatile used sometimes?
