#include <stdlib.h>
#include <stdio.h>
#include <ucontext.h>

int main(){
    register int done = 0;

    ucontext_t c1, c2;

    getcontext(&c1);
    printf("hello\n");

    if(!done){
        printf("in if \n");
        done = 1;
        swapcontext(&c2, &c1);
    }

    return 0;
}