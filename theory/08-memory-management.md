# Free Space Management
We need to know space management on two different levels: User-level (often times for the heap), and on an OS-level (often 
when implementing the space management/allocator for the entire memory). We have seen how space is managed on OS:s with segmentation (splitting 
a process into segments, remembering their base and bound for each segment etc). We will also cover how to manage the space
on an OS using paging (split memory into physical pages, and let each process be split into virtual pages that are mapped 
to them). We will now look into space management for the heap. Specifically, we will look at how space allocation and deallocation is handled.

When we allocate memory of size X on the heap (e.g using malloc), our heap needs to allocate the memory. This process makes use of 
the concept of splitting: It finds a free memory interval in the free list using some policy, and allocates the X first memory units of that 
interval, leaving the rest. It *splits* the interval. When memory is deallocated, that part of the memory is simply converted to an 
interval in the free list. When deallocating, two free intervals may end up free next to each other. It is very important to 
coalesce these intervals to repair the damage of external fragmentation.

## Allocation Policies
Best fit and Worst fit are two policies that find the smallest and largest intervals that satisfy the size constraints 
respectively, and use them for splitting. These have to go through all intervals if we assume nothing about the free list, 
which is linear in time. However, Best fit tends to leave small chunks of memory scattered. Worst fit also causes external
fragmentation, which has been empirically motivated. 

First fit takes the first satisfying interval, which is fast but uses up the memory unevenely (most of it in the beginning). Next 
fit is First fit, except you keep track of the last interval used and start the search from there next time, to find the "next 
first fit".

The binary buddy allocator algorithm is a more advanced approach, and does not require a list of the free memory. Instead, assume you have a binary memory. Then, you keep splitting the size 
in two until the size is big enough such that there exists a free memory area of that size able to contain the request and 
no smaller size is possible. This is relatively fast, logn actually. However it can have horrible internal fragmentation (about a third of the total space is lost). 
The buddy algorithm is elegant because when you deallocate memory, it is very easy to coalesce: Just deallocate, then check if 
the "buddy" is free (i.e the memory within the same "containing block"). If it is free, coalesce, and check the now bigger intervals' 
buddy if its free. What is a buddy och a block of size X? Well, it is another block of size X such that they form a block of 
size 2X. If you are in a "recursion depth" of k, then the buddy simply is the same address as the current block with the k:th
bit flipped. For instance, any two buddy leaves (deepest recursion) have the last bit flipped (counting from the left).

> It is important to stress that the binary buddy allocator is not related to the use of paging; it is simply a policy that decides how memory is split and coalesced (and allocated) on the main memory (as with all free-memory management). Paging is moreso about how to define virtual memory: Paging defines it in terms of pages.

## More on Managing Memory
When you allocate memory of size X, you don't actually allocate memory of size X. You allocate memory of size X+8 or something, where 8 symbolizes two bytes: A magic number (identifying the memory) and the size of the requested allocation (which would contain the value X in this case). So these extra values need to be taken into account when allocating. The 
pointer returned is the virtual base address of the remaining X addresses, i.e right after the magic number.

Also, every free interval in the free list makes up a node. Each node points to some "base" virtual address representing
the start of the interval. Each node then uses 8 bytes (two integers) to store the size of the interval (minus the 8 bytes 
from the two integers, which give the area of the "actual free space") and a pointer to the next free interval node. When 
converting a node into allocated memory from a request of size X, you take the node pointer, add 8 bytes (this time representing the two integers needed for the size of the request and the magic number), and return that pointer. When doing so, you move the node so that it points to the address just after the X+8 allocated bytes. Then, add 8 bytes (representing 
the new size of the new interval, as well as the pointer to the next free interval which you had previously).
