#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


// This version of mylloc keeps track of how many freed blocks we have
// and will also give us statistics on the length of these
// blocks. This is so that we can compare it with the others.



struct chunk {
  long int size;
  struct chunk *next;
};


struct chunk *flist = NULL; 

int length_of_free() {
  int i = 0;
  struct chunk *next  = flist;
  while(next != NULL) {
    i++;
    next = next->next;
  }
  return i; 
}


void sizes(int *buffer, int max) {
  struct chunk *next = flist;
  int i = 0;

  while((next != NULL) & (i < max)) {
    buffer[i] = next->size;
    i++;
    next = next->next;
  }
}



void *malloc(size_t size) {

  /* use sbrk to allocate new memory */
  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    return  (void*)(cnk + 1);
  }
}



void free(void *memory) {
  if(memory != NULL) {
    /* we're jumping back one chunk position */
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}



