/* should be compiled using gcc -c, since we do not want to link them. */
#include <stdlib.h> // for malloc
#include <unistd.h> // for unix standard stuff like syscall
#include <stdio.h> // for printf stuff

/**
 * Returns void pointer to allocated memory.
 */
void* malloc(size_t size){
    if(size == 0){
        return NULL;
    } else {
        void* memory = sbrk(size); // very bad to just keep allocating memory

        // sbrk sends -1 on error
        if(memory == (void *) -1){
            return NULL;
        } else {
            return memory;
        }
    }
}

void free(void* pointer){
    // should probably actually free it. Better to have like a free-list
    return; 
}
