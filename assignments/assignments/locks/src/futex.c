#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>

volatile int count;

volatile int mutex;

int futex_wait(volatile int *futexp) {
  return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
}
void futex_wake(volatile int *futexp) {
  syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}


int try(volatile int *lock) {
  __sync_val_compare_and_swap(lock,  0, 1);
}


int lock(volatile int *lock) {
  int susp = 0;
  while(try(lock) != 0) {
    susp++;
    futex_wait(lock);
  }
  return susp;
}

void release(volatile int *lock) {
  *lock = 0;
  futex_wake(lock);
}

typedef  struct args {int inc; int id; volatile int *mutex;} args;

void *increment(void *arg) {

  int inc = ((args*)arg)->inc;
  int id = ((args*)arg)->id;
  volatile int *mutex = ((args*)arg)->mutex;
  printf("start %d\n", id);
  int susp = 0;
  for(int i = 0; i < inc; i++) {
    susp = susp + lock(mutex);
    count++;
    release(mutex);
  }
  printf("done %d, suspended %d times\n", id, susp);  
}


int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("usage futex <inc>\n");
    exit(0);
  }

  int inc = atoi(argv[1]);
  
  pthread_t p1, p2;

  args a, b;

  a.inc = inc;
  b.inc = inc;

  a.id = 1;
  b.id = 2;

  a.mutex = &mutex;
  b.mutex = &mutex;

  pthread_create(&p1, NULL, increment, &a);
  pthread_create(&p2, NULL, increment, &b);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  printf("result is %d\n", count);
  
  return 0;
}

