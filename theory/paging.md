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

* valid bit: if page has not been used yet by the program it is marked invalid. If the process tries to 
access that page error will occur. You can thus not even allocate those pages in the first place, making sparse processes take up 
way less space.
* protection bits: R/W/E
* dirty bit: Signals if the page was modified since it was last brought to main memory. In that case, data may not be up to date.
* reference bit: Signals if page has recently been accessed, good for replacement policies. 

Remember that paging can become slow, since something that should take 1 memory access actually takes 2, since you need an 
extra memory access just to find the PFN (or rather, the PTE from which you get the PFN among other bits) from the linear page table.

# Two Major Problems
We see that there are two major problems: First of all, we perform a whole extra memory access for EACH instruction/explicit memory 
access. That is expensive. Also, the page table may take up a huge amount of memory. We need to make this more efficient.

## Table Lookaside Buffer - Solution to Slow Paging
> Note: TLB is essentially a fully associative cache (all entries are put into ways of only one existing set) for transforming virtual addresses to physical. There may in turn exist a data cache in the processor,
responsible for storing physical address-accesses made, using the concepts of spatial and temporal locality -- just as we learnt in the 
course IS1500. Thus, there may exist several caches but for entirely different purposes.

The table lookaside buffer, TLB for short, is a way to reduce the number of extra memory accesses needed for each virtual address translation. It is a cache that is used to store common PTE:s. The process of translating a virtual address is usually to try accessing 
the TLB using the VPN; if the read was successful and the PTE is not protected then use the corresponding PFN; otherwise, you insert the PTE of that VPN into the TLB and then retry the access algorithm, this time getting a TLB hit. 

This cache can either be managed by hardware (which would require a built-in flushing mechanism and hardware knowledge of the page table 
data structure such as its base register and internal structure) or by software. If by software, then there are a few things to 
note. First, the software would need a trap handler for a TLB miss. Second, the return-from-trap needs to retry the last instruction, as 
opposed to "the next" instruction for regular interruptions/traps (this requires the hardware to save different PC:s depending on the
exception, which shouldn't be too hard to implement into hardware). Lastly, the first instruction of the TLB miss trap handler must not 
yield an exception, since otherwise an infinite chain of exceptions might occur. This last one can be solved by letting the trap handler live in physical memory (so no virtual address) or reserve some entries in the TLB for permanently valid virtual-physical translations such as for this trap handler. These permanently valid translations are called *wired* and always hit.

The TLB doesn't just store the PTE: It also stores a valid bit (not the same as valid bit of page table; TLB valid says if the entry has 
a valid PTE in it), protection bits (like page table), dirty bit (since caches may have outdated information).

One last core thing that is stored in each TLB entry is an ASID (Address Space Identifier); it is like a PID, but for identifying an 
address space. This means the OS must also store an ASID value for each process. Thus the hardware also needs a register for the ASID
of the current process. This yields much better context switches than having to flush the TLB each context switch.

## Smaller Tables - Reducing the Memory Needed for Paging
-- TODO --

# One Third Problem
One more problem connected to TLB:s are how to choose a good replacement policy for not just TLB replacements but replacements in general. This will be tackled further when page swapping is discussed.
