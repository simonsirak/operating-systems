/* compile with gcc bench.c easy. gcc linker will look at provided files before looking at libraries */
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "rand.h"

#define ROUNDS 10
#define LOOP 100000
#define BUFFER 100

int main(){
    void* buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++){
        buffer[i] = NULL;
    }
    void* init = sbrk(0);
    void* current;

    printf("The initial top of the heap is %p", &init);

    for(int i = 0; i < ROUNDS; i++){
        for(int j = 0; j < LOOP; j++){
            int index = rand() % BUFFER;
            if(buffer[index] != NULL){
                free(buffer[index]);
            }
            size_t size = (size_t) request();
            int* memory;
            memory = malloc(size);
            if(memory == NULL){
                fprintf(stderr, "malloc failed\n");
                return 1;
            }

            buffer[index] = memory;
            *memory = 123;
        }
        current = sbrk(0);
        int allocated = (int) ((current - init)/1024); // dividing by 2^10 to yield kibibyte (Kilobyte)
        printf("%d\n", i);
        printf("Current top of the heap is %p\n", current);
        printf("\tIncreased by %d Kbyte\n", allocated);
    }

    return 0;
}