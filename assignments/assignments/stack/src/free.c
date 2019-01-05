#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  char *heap = malloc(20);

  printf("the heap variable at: %p\n", &heap);
  printf("contains a pointer: %p\n", heap);
  *heap = 0x61;
  printf("pointing to: 0x%x\n", *heap);      

  free(heap);

  char *foo = malloc(20);

  printf("the foo variable at: %p\n", &foo);
  printf("contains a pointer: %p\n",  foo);

  *foo = 0x61;
  printf("pointing to: 0x%x\n", *foo);

  *heap = 0x62;
  printf("or is it pointing to: 0x%x\n", *foo);      

  
  int pid = getpid();

  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);  

  

  return 0;
}
