#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/mman.h>


/* This implementation of malloc uses a strategy used by Doug Lea in the
   implementation of dlmalloc. This is the same strategy used in the
   default Linux allocator ptmalloc3. In this implementation we make
   things a bit easier and maintain everything aligned to 8 bytes.
 */


#define OVERHEAD 1      // 1 8-byte field, the overhead
#define MAGIC 2         // 2 8-byte fields in header
#define MIN 3           // 3 data in the smallest block 

#define HEAP  4*1024    // size () of memory allocated by mmap() 

#define FALSE 0 
#define TRUE 1


/* A block begins with a header that holds:

      size of block before
      size of the block 
      if we can merge with the block before
      if the block is free

   This head needs to be 8-byte aligned.
 */


/* A free block holds pointers for the double linked list of free
   blocks. 
 */

typedef struct block {

  uint64_t footer;     // size of block before    8 bytes
  uint32_t size;             // size of this block      4 bytes
  uint16_t merge;      // status of block before  2 bytes
  uint16_t free;       // status of this block    2 bytes

  struct block *next;   // next in list            8 bytes
  struct block *prev;   // previous in list        8 bytes

} block;

// only one free list, this could be changed

block *freelist;

/*
 These are the magic functions.
 */

static block *magic(void *memory) {
  return (block*)((uint64_t*)memory - MAGIC);
}

static void *hide(block* current) {
  return (void*) ((uint64_t*)current + MAGIC);
}

static block *locate_before(block *current) {
  assert(current->merge);
  int size = current->footer;
  return (block *)((uint64_t*)current -(size-1) -MAGIC);
}
  
static block *locate_after(block *current) {
  int size = current->size;  
  return (block*)((uint64_t*)current +MAGIC +(size-1));
}

static void set_footer(block *current) {
  block *after = locate_after(current);
  after->footer = current->size;
}

static int get_footer(block *current) {
  block *after = locate_after(current);
  return after->footer;
}

static int align(int size) {
  int rem =  (size % 8);
  if(rem != 0)
    size += 8 - rem;
  size >>= 3;
  if (size <= MIN)
    return MIN;
  else 
    return size;
}

#ifdef DEBUG
#define Trace(f_, ...) printf((f_), ##__VA_ARGS__)
#else
#define Trace(f_, ...) 
#endif

void sanity();

static block *new() {

  size_t length =  HEAP*sizeof(uint64_t);
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS;
  
  block *new = (block *)mmap(NULL, length, prot, flags, -1, 0);

  Trace("new\n");
  if( new == MAP_FAILED ) {
    Trace("mmap failed: %d\n", errno);
    return NULL;
  }
  Trace("allocating new space %p \n", new);
  int size = HEAP - MAGIC - OVERHEAD;
  :
  :
  // a sentinel that we should never merge with
  block *sentinel = locate_after(new);
  :
  :
  return new;
}

static  block *find(int size) {

  Trace("find block of size %d\n", size);  

  block *found = NULL;
  
  while(1) {
    found = freelist;

    while(found != NULL) {
      if (found->size >= size) {
        :
	Trace("found block of %p \n", found);        
	break;
      }
      found = found->next;
    }

    if(found == NULL) {
      block *heap = new();
      if(heap == NULL) {
	break;
      }
      :
    } else {
      break;
    };
  }
  return found;
}


void *dalloc(size_t bytes) {
  if(bytes == 0 ){
    return NULL;
  }
  int size = align(bytes);
  block *current = find(size);
  if(current == NULL)
    return NULL;  
  // sanity();
  return hide(current);
}


static  void merge(block *current) {
  // try to merge with before
  if(current->merge) {
    block *before = locate_before(current);
    block *after = locate_after(current);
    if (after->free) {
      :
      Trace("  merge with both before %p and after %p resulting in size %d\n", before, after, before->size);
    } else {
      :
      Trace("  merge with before %p resulting in size %d\n", before, before->size);
    }
    set_footer(before);
  } else {
    Trace("   before not free, ");
    block *after = locate_after(current);
    if (after->free) {
      :
      Trace(" merge with after (%p) resulting in size %d\n", after, current->size);
    } else {
      :
      Trace(" nor is after\n");
    }
    current->free = TRUE;
    set_footer(current);    
  }
}


void dfree(void *memory) {

  if(memory != NULL) {
    block *current = magic(memory);
    Trace("merging block %p of size %d\n", current, current->size);
    merge(current);
  }
  // sanity();
  return;
}

// Statistics

void statistics(FILE *out) {
  int length = 0;
  block *next = freelist;
  while(next != NULL) {
    next = next->next;
    length++;
  }
  printf("Length of free list: %d\n", length);
  
}

// Test sequences

void sanity() {
  block *next = freelist;
  block *prev = NULL;

  Trace("sanity \n");    
  while(next != NULL) {
    assert(next->free);
    assert(next->merge == FALSE);    
    assert(next->size >= MIN);
    assert(get_footer(next) == next->size);
    assert(next->prev == prev);

    block *after = locate_after(next);
    Trace("   free %p size %d after %p \n", next, next->size, after);    

    assert(after->free == FALSE);
    assert(after->merge == TRUE);

    prev = next;
    next = next->next;
  }
}


void test() {
  void *b1 = dalloc(4);
  void *b2 = dalloc(30);
  void *b3 = dalloc(140);
  
  dfree(b1);
  void *b4 =  dalloc(10);
  dfree(b2);  
  dfree(b3);
  void *b5 = dalloc(130);
}
