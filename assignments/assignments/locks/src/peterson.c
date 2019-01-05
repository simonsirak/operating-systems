#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

volatile int count = 0;

volatile int request[2] = {0,0};
volatile int turn = 0;

void lock(int id) {
  request[id] = 1;
  int other = 1-id;
  turn = other;
  while(request[other] == 1 && turn == other); // spin
}

void release(int id) {
  request[id] = 0;
}

typedef  struct args {int inc; int id;} args;

void *increment(void *arg) {
  int inc = ((args*)arg)->inc;
  int id = ((args*)arg)->id;
  printf("start %d\n", id);
  
  for(int i = 0; i < inc; i++) {
    lock(id);
    count++;
    release(id);
  }
}

int main(int argc, char *argv[]) {

  if(argc != 2) {
    printf("usage peterson <inc>\n");
    exit(0);
  }

  int inc = atoi(argv[1]);
  
  pthread_t p1, p2;

  args a, b;

  a.inc = inc;
  b.inc = inc;

  a.id = 0;
  b.id = 1;

  pthread_create(&p1, NULL, increment, &a);
  pthread_create(&p2, NULL, increment, &b);
  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  printf("result is %d\n", count);
  
  return 0;
}
