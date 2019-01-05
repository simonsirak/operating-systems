#include <stdlib.h>
#include <stdio.h>
#include "rand.h"

int main(int argc, char* argv[]){
    int loop = atoi(argv[1]);
    for(int i = 0; i < loop; i++){
        int size = request();
        printf("%d\n", size);
    }
}