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
