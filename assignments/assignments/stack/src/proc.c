#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global string";

int main() {

  printf("the global variable at: %p\n\n", &global);    

  unsigned long local = 0x42;

  printf("the local variable at: %p\n\n", &local);  
  
  int *heap = malloc(sizeof(int));
  *heap = 0x22;
  
  printf("the heap variable at: %p\n", &heap);
  printf("    pointing to: %p\n", heap);
  printf("    that holds : 0x%x\n\n", *heap);        

  
  int pid = getpid();

  printf("process id: %d\n", pid);  
  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);  

  return 0;
}
