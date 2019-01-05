#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <sys/mman.h>

//#define PAGESIZE (1)
//#define PAGESIZE (64)
#define PAGESIZE (4*1024)


//#define PAGES 64
//#define PAGES 128
//#define PAGES (32*1024)
#define PAGES (512*1024) 
// WARNING THIS MIGHT BE TOO LARGE
//#define PAGES (1024*1024) 


#define REFS (10*1024*1024)

/* This is an experiment to see if we can detect the size of the TLB */


int main(int argc, char *argv[]) {

  clock_t c_start, c_stop;

  struct timeval t_start, t_stop;

  volatile char *memory = malloc((long)PAGESIZE * PAGES);

  // We can experiment with huge-pages to see if it makes a
  // difference. In order for this to work you need to set the number
  // of huge-pages allocated by the system. Try 1001:
  //
  // > sudo bash -c "echo 1001 > /proc/sys/vm/nr_hugepages"
  //

  //volatile char *memory = (char*)mmap(NULL, (long)PAGESIZE*PAGES, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_HUGETLB, -1, 0);

  /* we run through the pages once so that we know that they are in memory */

  for(int p = 0; p < PAGES; p++) {
    long ref = (long)p * PAGESIZE;
    memory[ref] += 1;
  }    

  printf("#TLB experiment\n");
  printf("#  page size = %d bytes\n", (PAGESIZE));
  printf("#  max number of pages = %d\n",  PAGES);
  printf("#  total number or references = %d Mi\n", (REFS/(1024*1024)));
  printf("#  time for all references in sec \n"), 
  printf("#pages\t proc\t wall\t sum\n");  

  for(int pages = 1; pages <= PAGES; pages *=2) {

    int loops = REFS / pages;

    gettimeofday(&t_start, NULL);    

    c_start = clock();
    
    long sum = 0;
    for(int l = 0; l < loops; l++) {
      for(int p = 0; p < pages; p++) {
	long ref = (long)p * PAGESIZE;
	sum += memory[ref];
	//sum++;
      }
    }
    c_stop = clock();

    gettimeofday(&t_stop, NULL);    
    {
      double proc;
      long wall;

      proc = ((double)(c_stop - c_start))/CLOCKS_PER_SEC;

      wall =  (t_stop.tv_sec - t_start.tv_sec);

      printf("%d\t %.3f\t %ld\t %ld\n", pages, proc, wall, sum);
      
    }
  }
  return 0;
}


