#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>

#include <unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>


volatile int count = 0;

volatile int global = 0;

int futex_wait(volatile int *futexp) {
  return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
}
void futex_wake(volatile int *futexp) {
  syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}


int try(volatile int *mutex) {
  return __sync_val_compare_and_swap(mutex,  0, 1);
}


int lock(volatile int *mutex) {
  int spin = 0;
  while(try(mutex) != 0) {
    spin++;
    //sched_yield();
    // futex_wait(mutex);
  }
  return spin;
}

void release(volatile int *mutex) {
  *mutex = 0;
  // futex_wake(mutex);
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

  if(argc != 3) {
    printf("usage swip <threads> <inc>\n");
    exit(0);
  }

  int n = atoi(argv[1]);
  int inc = atoi(argv[2]);    

  args *as = malloc(n *sizeof(args));

  pthread_t * ps = malloc(n * sizeof(pthread_t));
  
  for(int i = 0; i < n; i++) {
    as[i].id = i;
    as[i].inc =  inc;
    as[i].mutex = &global;
  }

  for(int i = 0; i < n; i++) {
    pthread_create(&ps[i], NULL, increment, &as[i]);
  }

  for(int i = 0; i < n; i++) {
    pthread_join(ps[i], NULL);
  }
  
  printf("result is %d\n", count);
  
  return 0;
}

