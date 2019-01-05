// needed for pthread_yield
#define _GNU_SOURCE   

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

volatile int count = 0;

volatile int global = 0;

int try(volatile int *mutex) {
  return __sync_val_compare_and_swap(mutex,  0, 1);
}


int lock(volatile int *mutex) {
  int spin = 0;
  while(try(mutex) != 0) {
    spin++;
    // pthread_yield();   // not standard on Linux
    //sched_yield();
  }
  return spin;
}

void release(volatile int *mutex) {
  *mutex = 0;
}

typedef struct args {int inc; int id; volatile int *mutex;} args;

void *increment(void *arg) {

  int inc = ((args*)arg)->inc;
  int id = ((args*)arg)->id;
  volatile int *mutex = ((args*)arg)->mutex;
  printf("start %d\n", id);

  int spin = 0;
  
  for(int i = 0; i < inc; i++) {
    spin = spin + lock(mutex);
    count++;
    release(mutex);
  }

  printf("stop %d, spining %d\n", id, spin);  
}


int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("usage swap <inc>\n");
    exit(0);
  }

  int inc = atoi(argv[1]);
  
  pthread_t p1, p2;

  args a, b;

  a.inc = inc;
  b.inc = inc;

  a.id = 1;
  b.id = 2;

  a.mutex = &global;
  b.mutex = &global;

  pthread_create(&p1, NULL, increment, &a);
  pthread_create(&p2, NULL, increment, &b);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  printf("result is %d\n", count);
  
  return 0;
}

