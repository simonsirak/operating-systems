# Unix Address Space API
The Unix API for manipulating the different "growing" parts of a process are simple. All variables are pushed to the 
stack by the compiler, dying with the surrounding code block, and all malloc:s live indefinitely until the pointers 
are explicitly freed. 

Malloc is implemented using system calls for heap memory allocation, called brk and sbrk. brk explicitly changes the 
breakpoint, i.e the "end" of the heap, to a new given pointer. sbrk is a bit more flexible. 

Another (system?) call is mmap. Creating a memory using it will generate a memory area that is comparable to 
that of a heap, and can be managed as such.

## Malloc Errors
If you dont allocate memory and map it to a pointer (e.g use uninitialized pointer) you will prolly get segmentation 
fault. If you allocate but not enough, you will probably get a fault as well. If you allocate but don't initialize any 
value in the memory, you might get random values that happened to be there. If you forget to free, you will get a 
memory leak. You might also accidentally free to many times or free prematurely.
