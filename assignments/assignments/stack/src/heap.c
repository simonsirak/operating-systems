#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  char *heap = malloc(20);
  
  printf("the heap variable at: %p\n", &heap);
  printf("pointing to: %p\n", heap);      
  
  int pid = getpid();

  printf("process id: %d\n", pid);  
  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);  

  return 0;
}
