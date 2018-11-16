# Page Swapping
Swapping pages is a mechanism needed to allow the user to run more processes than main memory can handle. This is done by swapping pages 
out from main memory to a swap space located on the disk (secondary memory). This raises the need to save the disk address of a page 
whenever it is in the swap space.

But in order to know if a page is in the memory, we need to store a present bit as part of the PTE. If it is 0, then the page is in the 
swap space and we need to fetch it. In fact, if it is in swap space then the PTE can replace the PFN with the disk address. This page 
will then be swapped in using that address. If a page is not present, simply use that address to find and fetch it into main memory. If 
the memory is full, then you will need to evict a page before swapping in the desired page. 

Page evictions usually occur when some Lower Watermark has been reached (i.e less than X pages left). Then, pages are evicted 
based on some policy until a High Watermark has been reached.

# Swapping Policies
Swapping is esentially replacements for the pages of main memory, which is pretty much a fully associative cache for pages 
(like the TLB is for address translations). When studying cache, one could separate misses into compulsory (when the cache has 
been flushed), capacity (when the cache is full), and conflict miss (when all ways of a given set of the cache are taken). Conflict 
misses don't exist for fully associative caches.

One policy is the optimal one: evict the page that will be used furthest in the future. (One may think: Isn't it sometimes better 
to not evict that page at all then? Well in that case where would the next page to swapped in go? We will assume at least 1 page 
needs to be evicted each time we try to evict).

Another one is the FIFO policy. First one in is evicted first. However this totally disregards spatial and temporal locality, 
especially temporal. One upside is that this policy only updates the FIFO everytime a page is swapped in/out. This is still 
slow since lots of cache misses would be made for regular uses, since locality is so vital. Also, FIFO doesn't exhibit the 
stack property (don't fully understand this property), but it means that FIFO performs worse when cache gets bigger.

The random policy is a little bit better than FIFO. It evicts a random page.

Least recently used is a policy that uses historical data, which in turn means it utilizes locality (temporal locality; the spatial
locality comes from the use of pages). This performs almost as well as optimal when the workload exhibits locality-based behaviour.
However implementing LRU would require updating a doubly-linked list of page-history every memory acces, which is expensive. 
Instead, one can use a reference/use bit to approximate LRU. Here's one algorithm for it, the clock algorithm:

Each memory access sets the use bit of the corresponding page to 1. Then, we have a pointer to one of the allocated pages. When 
eviction is due, we check that pages entry. If it has use bit 1, then set that bit to 0 and say "if i catch u 1 mo'gain and u havent
been used, ur OUT, DONE". If it has use bit 0 and is caught, then it is evicted. In either case, the pointer goes to the next allocated 
page. 

## Thrashing
If we have waaay to many processes, then swapping would occur almost every context switch for instance. This would be too slow, and 
constant page swapping is called thrashing. In this case you should probably have some way of reducing the workload such as killing 
a huge process or letting certain processes finish so they can get out of the memory.
