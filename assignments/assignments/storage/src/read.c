#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

#define BLOCKS 1024*1024
#define SIZE 512
#define STEP 64  // can not be zero
#define COUNT 1000
#define SEED 1234

int compare(const void *a, const void *b) {
  return (int)(*(long *)a - *(long*)b);
}

long n_sec(struct timespec *start, struct timespec *stop) {
  long wall_sec = stop->tv_sec - start->tv_sec;
  long wall_nsec = (wall_sec *1000000000) + stop->tv_nsec - start->tv_nsec;
  return wall_nsec;
}

	  
int main(int argc, char *argv[]) {
  
  if(argc < 2) {
    printf("usage: read  <file name>  [<seed>]\n");
    return -1;
  }


  
  char *name = argv[1];

  int seed = SEED;  
  if(argc == 3) seed = atoi(argv[2]);

  srand(seed);
  
  int flag = O_RDWR | O_CREAT | O_DSYNC;  
  int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
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
      int buffer;
      clock_gettime(CLOCK_MONOTONIC, &t_start);
      read(fd, &buffer, sizeof(int));
      clock_gettime(CLOCK_MONOTONIC, &t_stop);
      lseek (fd, STEP - sizeof(int), SEEK_CUR);
      table[e][c] = n_sec(&t_start, &t_stop);
    }
  }
  printf("# read access\n#\n");
  printf("#%d random selected blocks were accessed.\n", COUNT);
  printf("#The block size: %d.\n", SIZE);
  printf("#The number of blocks:  %d.\n", BLOCKS);      
  printf("#Each block was accessesd in a  sequence of %d read operations separated by %d bytes.\n", entries, STEP);
  printf("#The measurments show time in ns to complete a read opeation. N is the index in the block.\n#\n");

  printf("#N\tMin\tQ1\tMed\tQ3\tD9\tMax\n");    
  for(int e = 0; e < entries; e++) {
    qsort(table[e], COUNT, sizeof(long), compare);
    long min = table[e][0];
    long q1  = table[e][COUNT/4];
    long med = table[e][COUNT/2];
    long q3  = table[e][3*(COUNT/4)];
    long d9  = table[e][9*(COUNT/10)];    
    long max = table[e][COUNT-1];
    printf("%d\t%ld\t%ld\t%ld\t%ld\t%ld\t%ld\n", e*STEP, min, q1, med, q3, d9, max);
  }
  return 0;
}


 








