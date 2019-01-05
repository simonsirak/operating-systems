/* should be compiled using gcc -c, since we do not want to link them. */
#include <stdlib.h> // for malloc
#include <unistd.h> // for unix standard stuff like syscall
#include <assert.h>

/*
    A free list is a list of free memory chunks.
    This in particular is a singly linked list.

    Each allocated memory chunk will contain the 
    chunk itself right before the actual memory.

    This is done when we allocate.

    The awesome thing is that the free list is 
    integrated into the heap! With each free chunk,
    we dedicate a small piece in the beginning to
    the free list chunk node.
*/
struct chunk {
    int size;
    struct chunk *next;
};

struct chunk* flist = NULL;

/**
 * Returns void pointer to allocated memory.
 */
void* malloc(size_t size){
    if(size == 0){
        return NULL;
    } else {
        size_t actual = size + sizeof(struct chunk);

        // search through free list
        struct chunk* cnk = flist;
        struct chunk* prev = NULL;
        while(cnk != NULL){
            if(cnk->size >= size){ // doesn't need to be "+ size of chunk" because cnk already is in the beginning of the free chunk, so the chunk space is already accounted for.
                if(prev != NULL){ // if cnk is between two chunks
                    prev->next = cnk->next;
                } else { // if cnk is first chunk
                    flist = cnk->next;
                }
                // do stuff
                return (void*)(cnk+1); // return pointer to memory
            } else {
                prev = cnk;
                cnk = cnk->next;
            }
        }

        void* memory = sbrk(actual); // allocate more space since we need it

        // sbrk sends -1 on error
        if(memory == (void *) -1){
            return NULL;
        } else {
            struct chunk* cnk = (struct chunk*) memory;
            cnk->size = size; // just the size of the actual block we wanted.
            return (void*)(cnk+1);
        }
    }
}

void free(void* pointer){
    if(pointer != NULL){
        struct chunk* cnk = (struct chunk*) pointer - 1;
        cnk->next = flist; // put at beginning of free list (unordered)
        flist = cnk;
    }
    return; 
}
