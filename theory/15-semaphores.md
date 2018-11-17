# Semaphores
Semaphores are an alternative to locks and condition variables. A semaphore is essentially a shared integer that is atomically
modified and examined at the same time. It has an initial shared value, and two functions `wait` and `post`. Waiting decrements the shared value and puts the thread to sleep if the new value is negative. Posting increments the value and 
wakes one thread if there is one asleep.

It can be used as a lock if the initial value is 1, and can be used for ordering if the initial value is 0.

There are a few crucial differences between semaphores and locks/CVs:

* Waiting does not require you to release any held lock, and does not require you to hold a lock prior to releasing.
Thus, if a lock is held by one thread that waits, and a second thread tries to grab that lock, it'll be forced to sleep as well. If the second is the only thread that can wake the first thread, this may lead to deadlock. This could happen in a 
typical semaphore implementation of the Producer/Consumer pattern. 
* Because no locks are required when using semaphores (in particular, when the initial value is > 1), unlucky context 
switches might lead to two threads being in the same critical section at once. And we know locks (e.g semaphores with
initial value = 1) are required for critical sections to prevent these race conditions.

## Producer/Consumer Using Semaphores
Taking note of these differences, we can implement a simple Producer/Consumer pattern according to the book. We need to 
simulate the holding/releasing of locks similar to condition variables. Fortunately, this does not need to be purely 
atomic (an context switch occuring right after `wait(&full)` **returns** and right before the lock grab is not the end of the world; if it switches to another consumer, then it will pass through its own `wait(&full)` as well and grab the lock, however there will still be another item left for the first consumer; if it instead switches to a producer, no harm will be done either). We see that the lock is only ever held if there is a guarantee that the holder can wake at least one other thread. In 
a sense, the basic semaphores ensure the overall behaviour of the implementation, whereas the lock does its basic task of ensuring that "once a single producer/consumer actually tries to do the job that the semaphores guaranteed it should be able to do, it is protected by a lock".

Having two semaphores, one for fullness and one for emptyness, ensures that we can control the upper and lower limits (0 and 
MAX), thus controlling where context switches will go when the buffer is full/empty. This is similar to the CV implementation.
If a semaphore could check against two values instead of just "< 0", then only one semaphore would be necessary. However that 
destroys part of the generality of semaphores.

```C

int buffer[MAX]; // shared resource only modified with another shared resource, namely the semaphore value.
int fill = 0; // for the circular queue
int use = 0; // for the circular queue

void put(int value) {
  buffer[fill] = value; 
  fill = (fill + 1) % MAX;
}

int get() {
  int tmp = buffer[use]; 
  use = (use + 1) % MAX;
  return tmp;
}

sem_t full; // semaphore for determining the fullness of the buffer, init to 0  
sem_t empty; // determines the emptyness of the buffer, init to MAX
sem_t lock; // lock to protect critical section, init to 1

void *producer(void *args){
  while(1){
    wait(&empty);
    wait(&lock);
    put(42);
    post(&lock);
    post(&full);
  }
}

void *consumer(void *args){
  while(1){
    wait(&full);
    wait(&lock);
    get();
    post(&lock);
    post(&empty);
  }
}
```
