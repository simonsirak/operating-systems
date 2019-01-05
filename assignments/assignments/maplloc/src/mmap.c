#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>



int main() {

  void *s0 = sbrk(0);
  printf("heap at: %p\n", s0);

  mmap(NULL, 1024, PROT_WRITE, MAP_ANON, 0, 0);

  void *s1 = sbrk(0);
  printf("heap at: %p\n", s1);

  int t = (int)(s1 - s0);
  printf("total : %d\n", t);  
}
