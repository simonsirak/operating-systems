#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

char global[] = "This is a global string";

const int read_only = 123456;

int main() {

  const int local_readonly = 654321;

  int pid = getpid();

  
 foo:
  printf("process id: %d\n", pid);

  printf("global string: 0x%lx\n", (unsigned long)&global);

  printf("read only:  0x%lx\n", (unsigned long)&read_only);  
  printf("local read only:  0x%lx\n", (unsigned long)&local_readonly);  

  printf("the code: 0x%lx\n", (unsigned long)&&foo);    

  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);  

  return 0;
}
