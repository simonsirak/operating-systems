# Paging
Paging is really just simplified memory management. The memory is chopped up into fixed size blocks called physical pages.
Each process then consists of a number of virtual pages. Each of these are mapped to a physical table in main memory via 
a page table, which is stored by the OS (because it is usually huge and cannot fit on hardware directly). A page table 
contains a mapping (translation) from each virtual table to a physical table. Which physical page is determined by the first few bits (counting 
from the left) of the virtual address, and the remaining bits describe the offset within the physical table. The rest is 
a lot like base-bounds.

If we assume a linear page table (an array where array[VPN] returns the PFN, physical frame number aka physical page number), 
each such entry in the array contains one integer. Also assume the page table lives in physical memory and the OS keeps a physical 
address of its base. The first majority of bits contain the PPN/PFN. The remaining bits contain data such as:

* valid bit: if page has not been used yet by the program it is marked invalid. If the process tries to access that page error will occur.
* protection bits: R/W/E
* dirty bit: Signals if the page was modified since it was last brought to main memory. In that case, data may not be up to date.
* reference bit: Signals if page has recently been accessed, good for replacement policies. 

Remember that paging can become slow, since something that should take 1 memory access actually takes 2, since you need an 
extra memory access just to find the PFN (or rather, the PTE from which you get the PFN among other bits) from the linear page table.

# Two Major Problems
We see that there are two major problems: First of all, we perform a whole extra memory access for EACH instruction/explicit memory 
access. That is expensive. Also, the page table may take up a huge amount of memory. We need to make this more efficient.
