#include <stdlib.h>
#include <stdio.h>

int main() {

 foo:

  printf("the code: %p\n", &&foo);  

  fgetc(stdin);
  return 0;
  
}

