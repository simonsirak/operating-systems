#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* the number of data points */
#define SAMPLES 20

/* 20% of the pages will have 80% of the references */  
#define HIGH 20
#define FREQ 80

typedef struct pte {
  int present;
} pte;


void init_sequence(int *sequence, int refs, int pages)  {

  int high = (int)(pages*((float)HIGH/100));

  /* dummy value */
  int prev = pages; 
  
  for(int i = 0; i<refs; i++) {
    if(rand()%100 < FREQ) {    
      int rnd;
      do {
	/* we want to prevent the same page being picked again */
	rnd = rand()%high;
      } while (rnd == prev);
      prev = rnd;
      sequence[i] = rnd;          
    } else {
      int rnd;
      do {
	rnd = high + rand()%(pages - high);
      } while (rnd == prev);
      prev = rnd;
      sequence[i] = rnd;
    }
  }
}

void clear_page_table(pte *page_table, int pages) {
    for(int i=0; i < pages; i++) {
      page_table[i].present = 0;
    }
}

int simulate(int *sequence, pte *page_table, int refs, int frames, int pages) {

  int hits = 0;
  int allocated = 0;
    
  for(int i = 0; i < refs; i++) {
    int next = sequence[i];
    pte *entry = &page_table[next];

    if(entry->present == 1) {
      hits++;
    } else {
      if(allocated < frames) {
	allocated++;
	entry->present = 1;
      } else {
	pte *evict;

	/* initial values */
	int sofar = 0;
	int candidate = pages;
	
	for(int c = 0; c < pages; c++) {
	  if(page_table[c].present == 1) {
	    /* the page is allocated */
	    int dist = 0;
	    while(sequence[i+dist] != c && i+dist < refs) {
	      dist++;
	    }
	    if(dist > sofar) {
	      candidate = c;
	      sofar = dist;
	    }
	  }
	}
	evict = &page_table[candidate];

	evict->present = 0;
	entry->present = 1;
      }
    }
  }
  
  return hits;
}


int main(int argc, char *argv[]) {


  /* the number of references, should be a command line argument */
  int refs = 100000;

  /* the number of pages of a process, should be a command line argument */
  int pages = 100;
  
  int *sequence = (int*)malloc(refs*sizeof(int)); 

  init_sequence(sequence, refs, pages);

  pte *page_table = (pte *)malloc(pages*sizeof(pte)); 

  printf("# This is a benchmark of optimal replacement policy\n");
  printf("# %d page references\n", refs);    
  printf("# %d pages \n", pages);  
  printf("#\n#\n#frames\tratio\n");    
  
  /*  frames is the size of the memory in frames */
  int frames;

  int incr = pages/SAMPLES;
  
  for(frames = incr; frames <= pages; frames += incr) {

    /* clear page tables entries */
    clear_page_table(page_table, pages);

    int hits =  simulate(sequence, page_table, refs, frames, pages);

    float ratio = (float)hits/refs;

    printf("%d\t%.2f\n", frames, ratio);
  }

  return 0;
}
