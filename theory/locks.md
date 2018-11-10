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

## 
