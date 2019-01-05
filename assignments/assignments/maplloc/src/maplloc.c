#include <stdlib.h>
#include <sys/mman.h>

#define HUGE 64*1024*4096

char *heap;

char *brkp = NULL;
char *endp = NULL;


static void init() __attribute__((constructor));

void init() {
  heap = (char *)mmap(NULL, HUGE,  (PROT_READ | PROT_WRITE), (MAP_PRIVATE | MAP_ANONYMOUS), -1, 0);
  brkp = heap;
  endp = brkp + HUGE;
}

void *sbrk(size_t size) {
  if(size == 0) {
    return (void *)brkp;
  }
  void *free = (void *)brkp;

  brkp += size;
  if(brkp >= endp) {
    return NULL;
  }
  return free;
}


void *malloc(size_t size) {
  /* use sbrk to allocate new memory */
  void *memory = sbrk(size);
  if(memory == (void *)-1) {
    return NULL;
  } else {
    return memory;
  }
}


void free(void *memory) {
  /* do nothing */
  return;
}
