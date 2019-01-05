#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>

#define SEED 1234


int main(int argc, char *argv[]) {
  
  int seed = SEED;
    
  if(argc = 2) seed = atoi(argv[1]);

  srand(seed);
  
  for(int i = 0;i < 10; i++) {
    printf("rand %d\n", rand());
  }
}
  



