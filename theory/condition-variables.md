# Condition Variables
Condition variables are a way to ensure a certain condition is met before proceeding with a thread. Condition variables are 
meant to optimize that specific task, as opposed to having a simple "spin-lock" that polls for a certain condition and wastes 
cycles. 

We decide that CVs should have the simple interface of `wait` and `signal`. Waiting is done when we realise a shared resource
does not have the required value that we want at that point of execution for that particular thread. We then sleep on a certain
condition until some other thread signals on that condition variable, usually meaning that the corresponding shared resource 
has been changed and one of the threads waiting on that condition should wake up. For now, we will only consider having one 
shared resource to work around. I have posed a question on Canvas that asks for the case of several interacting shared resources,
and am awaiting an answer.

In order to ensure correctness, we have a few rules/definitions to follow:

* You usually have a variable that records some sort of shared state/resource -- i.e that checks some condition. It is actually 
pretty much essential that you have one that the condition variable works around. Furthermore, the actual checking of the 
state of that shared resource should be done in a while-loop, as opposed to an if-statement. This fixes the issue of having 
a consumer steal the last produced value from another sleeping consumer; if you only checked using an `if`-statement, the 
stolen from consumer would think that the stolen produce is still available, tries to grab it, but nothing is there -- an
undesireable scenario.
* You should have ONE lock associated with the shared resource. The shared resource may in turn be associated with 
several condition variables. The shared resource and CV should only be modified if the thread has acquired that lock. This 
lock ensures that a condition variable is only modified/read by at most one thread at a time. The lock must be held by the 
thread that calls `wait`, during which the lock is released while putting the thread to sleep (atomically). Signaling will 
return one thread from its sleep call, and reacquire the lock (also atomically).
* When you want to specify which threads to sleep on which condition, it is much better to have condition variables that 
separate these threads. It is possible for these CVs to work in harmony. We will see this in the Consumer/Producer problem.
This handles the fact that the fairness when waking one thread using signal usually depends on the underlying data structure 
for storing sleeping threads.

Let's try using these rules in an implementation of the Producer/Consumer problem.

## The Producer Consumer Problem
The problem describes the problem of letting producers and consumers access a shared buffer, each producer/consumer working 
as a separate thread. We will implement this using locks and CVs as described by the suggested rules above. I actually jumped 
the gun on some tips for implementation by writing the above rules. But let's implement it! We will later see an implementation using Semaphores as well.

Assume we have some shared resource `int buffer` which can contain 1 item. Then the following code should work:

```C
lock_t lock;
cond_t fill, empty;
int buffer; // shared resource that only changes when the shared resource 'count' changes
int count = 0; // shared resource that dictates the condition

void put(int value){
  assert(count == 0); // only produce on a non-full buffer
  count++;
  buffer = value;
}

int get(){
  assert(count == 1); // only consume from a non-empty buffer
  count--;
  return buffer;
}

void produce(){
  while(1){
    lock(&lock); // attempt to read/write to shared resources, so must lock
    
    while(count == 1){ // wait if buffer is full
      wait(&empty, &lock); // wait until the someone has issued an "emptied" signal
    }
    
    put(42); // put some value now, since we know the buffer has room.
    signal(&fill); // signal that the buffer has been filled with something.
    
    unlock(&lock);
  }
}

void consume(){
  while(1){
    lock(&lock); // attempt to read/write to shared resources, so must lock
    
    while(count == 0){ // wait if buffer is empty
      wait(&fill, &lock); // wait until the someone has issued a "filled" signal
    }
    
    printf("%d", get()); // get value now, since we know buffer has room.
    signal(&empty); // signal that the buffer has been partially emptied by 1 unit.
    
    unlock(&lock);
  }
}
```

This code follows the design suggestions from the bullet points above. Fortunately, these are the only issues we need to 
keep in mind for this particular problem. In order to expand to a buffer of arbitrary size, you need to replace the buffer 
with an array and use it as a circular queue/ring buffer. The mechanics themselves are the same though.
