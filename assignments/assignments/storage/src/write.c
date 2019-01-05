#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

#define BLOCKS 1024*1024
#define SIZE 512
#define STEP 64  // can not be zero
#define COUNT 100
#define SEED 1234

int compare(const void *a, const void *b) {
  return (int)(*(long *)a - *(long*)b);
}

long nsec(struct timespec *start, struct timespec *stop) {
  long wall_sec = stop->tv_sec - start->tv_sec;
  long wall_nsec = (wall_sec *1000000000) + ((stop->tv_nsec - start->tv_nsec));
  return wall_nsec;
}

	  
int main(int argc, char *argv[]) {

  if(argc < 2) {
    printf("usage: access  <file name>  [<seed>]\n");
    return -1;
  }

  
  char *name = argv[1];

  int seed = SEED;
  if(argc == 3) seed = atoi(argv[2]);

  srand(seed);

  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  // int flag = O_RDWR | O_CREAT;
  int flag = O_RDWR | O_CREAT | O_DSYNC;  
  int fd = open(name, flag, mode);

  assert(fd != -1);

  int entries = SIZE / STEP;

  long **table = malloc(sizeof(long *) * entries);

  for(int i = 0; i < entries; i++) {
    table[i] = malloc(sizeof(long) * COUNT);
  }

  for(int c=0; c < COUNT; c++) {
    int b = rand() % BLOCKS;
    lseek(fd, b*SIZE, SEEK_SET);
    for(int e = 0; e < entries; e++) {
      struct timespec t_start, t_stop;
      int buffer = e;
      clock_gettime(CLOCK_MONOTONIC, &t_start);
      write(fd, &buffer, sizeof(int));
      clock_gettime(CLOCK_MONOTONIC, &t_stop);
      lseek (fd, STEP - sizeof(int), SEEK_CUR);
      table[e][c] = nsec(&t_start, &t_stop);
    }
  }

  printf("# write access\n#\n");
  printf("#%d random selected blocks were accessed.\n", COUNT);
  printf("#The block size: %d.\n", SIZE);
  printf("#The number of blocks:  %d.\n", BLOCKS);    
  printf("#Each block was accessed in a sequence of %d write operations separated by %d bytes.\n", entries, STEP);
  printf("#The measurments show time in ns to complete a write operation. N is the index in the block.\n#\n");
  printf("#N\tMin\tQ1\tMed\tQ3\tD9\tMax\n");    
  for(int e = 0; e < entries; e++) {
    qsort(table[e], COUNT, sizeof(long), compare);
    int q = COUNT/4;
    long min = table[e][0];
    long q1  = table[e][q];
    long med  = table[e][2*q];
    long q3  = table[e][3*q];
    long d9  = table[e][9*(COUNT/10)];    
    long max  = table[e][COUNT-1];
    printf("%d\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", e*STEP, min, q1, med, q3, d9, max);
  }

  return 0;
}


 








