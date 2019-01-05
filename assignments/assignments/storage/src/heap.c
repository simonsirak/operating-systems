#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#define BLOCKS 1024*1024
#define SIZE 512
#define STEP 64
#define COUNT 100

int compare(const void *a, const void *b) {
  return (int)(*(long *)a - *(long*)b);
}

long nsec(struct timespec *start, struct timespec *stop) {
  long wall_sec = stop->tv_sec - start->tv_sec;
  long wall_nsec = (wall_sec *1000000000) + ((stop->tv_nsec - start->tv_nsec));
  return wall_nsec;
}

int main(int argc, char *argv[]) {

  char *heap = malloc(BLOCKS*SIZE);
  
  int entries = SIZE / STEP;

  long **wtable = malloc(sizeof(long *) * entries);

  for(int i = 0; i < entries; i++) {
    wtable[i] = malloc(sizeof(long) * COUNT);
  }

  struct timespec t_prev, t_next;

  struct timespec *prv = &t_prev;
  struct timespec *nxt = &t_next;  

  int buffer;

  clock_gettime(CLOCK_MONOTONIC, prv);
  
  for(int c=0; c < COUNT; c++) {
    int b = rand() % BLOCKS;
    int pos = b*SIZE;
    for(int e = 0; e < entries; e++) {
      buffer = e;
      memcpy(&heap[b*SIZE + e*STEP], &buffer, sizeof(int));      
      clock_gettime(CLOCK_MONOTONIC, nxt);
      wtable[e][c] = nsec(prv, nxt);
      struct timespec *tmp = prv;
      prv=nxt;
      nxt = tmp;
    }
  }

  printf("# write access\n#\n");
  printf("#%d random selected blocks were accessed.\n", COUNT);
  printf("#The block size is %d.\n", SIZE);  
  printf("#Each block was accessed in a sequence of %d write operations separated by %d bytes.\n", entries, STEP);
  printf("#The measurments show time in ns to complete a write opeation. N is the index in the block.\n#\n");
  printf("#N\tMin\tQ1\tMed\tQ3\tD9\tMax\n");    
  for(int e = 0; e < entries; e++) {
    qsort(wtable[e], COUNT, sizeof(long), compare);
    int q = COUNT/4;
    long min = wtable[e][0];
    long q1  = wtable[e][q];
    long med  = wtable[e][2*q];
    long q3  = wtable[e][3*q];
    long d9  = wtable[e][COUNT - (COUNT/10)];    
    long max  = wtable[e][COUNT-1];
    printf("%d\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", e*STEP, min, q1, med, q3, d9, max);
  }

  return 0;
}


 








