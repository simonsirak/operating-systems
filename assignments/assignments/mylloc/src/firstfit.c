#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#define MIN 32
#define ALIGN 8

struct chunk {
  int size;
  struct chunk *next;
};

struct chunk *flist = NULL; 


/* sanity check : make sure that the free-list is ok */

int sanity() {
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while(next != NULL ) {
    char *check = (char*)next  + next->size + sizeof(struct chunk);
    assert((char*)prev < (char*)next);
    assert(check != (char*)next->next);
    prev = next;
    next = next->next;
  }
  return 1;
}

/* return the lenth of the free-list, only for statistics */

int length_of_free() {
  int i = 0;
  struct chunk *next  = flist;
  while(next != NULL) {
    i++;
    next = next->next;
  }
  return i; 
}

/* collect the sizes of all chunks in a buffer */

void sizes(int *buffer, int max) {
  struct chunk *next = flist;
  int i = 0;

  while((next != NULL) & (i < max)) {
    buffer[i] = next->size;
    i++;
    next = next->next;
  }
}


int align(int size) {
  int rem = size % ALIGN;
  if(rem > 0) 
    size +=  (ALIGN - rem);
  return size;
}

/* The free-list is ordered in address order. */

void insert(struct chunk *cnk) {
  struct chunk* next = flist;
  struct chunk *prev = NULL;

  while(next != NULL) {
    if(cnk < next)
      break;
    prev = next;
    next = next->next;
  }
  if(prev == NULL)
    flist = cnk;
  else 
    prev->next = cnk;
  cnk->next = next;
}

/* if possible, coalece with other free blocks */

void coalesce(struct chunk *cnk) {
  struct chunk* next = flist;
  struct chunk *prev = NULL;

  /* cnk_check is the adjacent block with higher address */
  char *cnk_check = (char*)cnk  + cnk->size + sizeof(struct chunk);

  /* prev_check is the adjacent block of the previous block */
  char *prev_check = NULL;

  while (next != NULL) {
    if(cnk_check == (char*)next) {
      if(prev_check == (char*)cnk) {
	// coalesce with both chunks
	prev->size += cnk->size + next->size;
	prev->next = next->next;
      } else {
	// coallece with next chunk
	cnk->size += next->size;
	cnk->next = next->next;
	if(prev == NULL){
	  flist = cnk;
	} else {
	  prev->next = cnk;
	}
      }
      return;
    }
    if(prev_check == (char*)cnk) {
      // coallece with previous chunk
      prev->size += cnk->size;
      prev->next = next;
      return;
    }
    if(next > cnk) {
      // regular insert 
      cnk->next = next;
      if(prev == NULL){
	flist = cnk;
      } else {
	prev->next = cnk;
      }
      return;
    }
    prev = next;
    prev_check = ((char*)prev  + prev->size + sizeof(struct chunk));
    next = next->next;
  }
  if(prev_check == (char*)cnk) {
    // coallece with final chunk
    prev->size += cnk->size;
    prev->next = next;
    return;
  } 
  //  insert as final chunk
  cnk->next = next;
  if(prev == NULL){
    flist = cnk;
  } else {
    prev->next = cnk;
  }
}



chunk *find(size_t int) {

  struct chunk *next = flist;
  struct chunk *prev = NULL;
  struct chunk* taken = NULL;

  while(next != NULL) {
    if (next->size >= size) {
      taken = next;
      if(prev != NULL) {
	prev->next = taken->next;
      } else {
	flist = taken->next;
      }
      break;
    }
    prev = next;
    next = next->next;
  }
  return taken;
}


/* We name the function balloc() and bree() to avoid clashes with the
   regular malloc()/free() */


void *balloc(size_t size) {
  if( size == 0 ){
    return NULL;
  }

  /* we only handle size of multiples of ALIGN */
  size = align(size);
  
  taken = find(size);

  if(taken != NULL) {
    /* left over is total size of remaining chunk  */
    int leftover = (taken->size - size);

    if(leftover >= MIN) {
      // new is the leftover chunk 
      struct chunk *new = (struct chunk*)(((char*)next) + size + sizeof(struct chunk));

      new->size = (leftover - sizeof(struct chunk));
      // leftover is inserted in free list 
      insert(new);

      // we're now changing the size of taken
      taken->size = size;
    }
    return  (void*)(next + 1);
  }
  /* use sbrk to allocate more memory */
  void *memory = sbrk(size + sizeof(struct chunk));
  if(memory == (void *)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    return  (void*)(cnk + 1);
  } 
}


void bree(void *memory) {

  if(memory != NULL) {
    /* we're jumping back one chunk position */
    struct chunk *cnk = (struct chunk*)(((struct chunk*)memory) - 1);
    coalesce(cnk); 
  }
  return;
}


