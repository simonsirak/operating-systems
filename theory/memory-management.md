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

The binary buddy allocator algorithm is a more advanced approach. Assume you have a binary memory. Then, you keep splitting the size 
in two until the size is big enough such that there exists a free memory area of that size able to contain the request and 
no smaller size is possible. This is relatively fast, logn actually. However it can have horrible internal fragmentation. 
The buddy algorithm is elegant because when you deallocate memory, it is very easy to coalesce: Just deallocate, then check if 
the "buddy" is free (i.e the memory within the same "containing block"). If it is free, coalesce, and check the now bigger intervals' 
buddy if its free. What is a buddy och a block of size X? Well, it is another block of size X such that they form a block of 
size 2X. If you are in a "recursion depth" of k, then the buddy simply is the same address as the current block with the k:th
bit flipped. For instance, any two buddy leaves (deepest recursion) have the last bit flipped (counting from the left).

## More on Managing Memory
