#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void zot(unsigned long *stop ) {

  unsigned long r = 0x3;
  printf("    r: %p\n", &r);
  
  // if we assume stop was created on the stack previous to 
  // the call to zot, then it will be on a larger address than &r.

  unsigned long k = 0x8;
  printf("    k: %p\n", &k);
  
  /*
  What gets printed, in order:
  address of r
  address of i (On my version of GCC at least; on this version, local variables within a function are in increasing order, and variables between nested functions are in decreasing order (i.e the addresses become smaller for each nested function).)
  stack pointer and 
  */
  unsigned long *i;
  printf("    i: %p\n", &i);

  unsigned long j = 0x6;
  printf("    j: %p\n", &j);

  for(i = &r;  i <= stop; i++){
    printf("%p      0x%lx\n", i, *i);
  }
}

void foo(unsigned long *stop ) {
  unsigned long q = 0x2;
  zot(stop);

  ret:
    printf("  ret: %p \n", &&ret);
}


int main() {

  unsigned long p = 0x1;

  int pid = getpid();
  printf("  pid: 0x%lx\n", &pid);

  unsigned long *i;
  /*for(i = &p - 2;  i >= (&p - 12); i--){
    *i = 0x00;
  }*/
  
  foo(&p);

 back:
  printf("  p: %p \n", &p);
  printf("  back: %p \n", &&back);

  printf("\n\n /proc/%d/maps \n\n", pid);
  char command[50];
  sprintf(command, "cat /proc/%d/maps", pid);
  system(command);  

  //sleep(20);
  return 0;
}





