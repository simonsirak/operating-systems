# Address Space
An address space is an abstraction that lets each process have its own virtual addresses that get translated 
to physical addresses in main memory. This translation is usually done with the help of hardware, in which it is then 
called dynamic translation. 

The abstraction came from the need of implementing time-sharing (e.g context switches) while 
not having to load processes from disk every context switch. The concept of an address space is thus to tackle the problem of having protection between processes while maintaining efficiency (time and spacewise). However the abstraction should be 
transparent, i.e users should be unaware of it.

How are address spaces usually implemented? Below are increasingly complex
additions to construct the abstraction.

## Basic Implementation
A simple and rather efficient solution is hardware-based address translation. Think of each process as 3 pieces: Code, 
Stack, Heap. Let all of these be wrapped in some memory block where code starts at the top (row 0), heap is underneath and stack starts at the bottom (row x) with the stack and heap growing towards each other. Then, the process can access certain addresses using its own local system from 0 to x (called a virtual address). In order to get the corresponding real memory address, we acquire the 
base physical address of the process and add it to the virtual address, given that the virtual address is less than the bounds
(i.e the size of the process). This check is done by the hardware, hence the name. It is a type of dynamic address translation. 

The system addresses effincient translation, protection between processes and the user is oblivious of the physical addresses.
However, the hardware (processor) must have registers for a base register and bound register, an interrupt for out-of-bounds or attempting priviliged operations, a priviliged mode, and also a priviliged instruction for registering a trap handler of an interrupt. The OS must in turn provide these trap handlers, and then store the base/bound for each process. The OS must also 
keep track of free memory, usually via a free list (list of free memory intervals).

## Segmentation: A Step Up
Segmentation aims to tackle one flaw in the pure base-bounds approach; internal fragmentation, i.e when memory within a 
process stays unutilized. The core of segmentation is to literally split the process into segments; one for code, heap, 
stack for instance. We can then keep track of base-bound pairs for each segment in the processor, and thus place them 
anywhere in memory, reducing the internal fragmentation. In order to know which segment is being addressed by a virtual 
address, we let the first few bits of the virtual address be sort of "indices" for the different segments. The aim is 
to have all indices covered by some actual segment that is used, otherwise memory will go unused. So for instance if we 
have the segments code, static, heap, stack, then 00 xxxxx could refer to any code address, 01 xxxxx for any static address 
etc.. Using "indices" is an explicit approach. An implicit approach would be for the hardware to figure out the type of address from the context. 

However, in doing this we also need to consider the "growing" direction. Thus, hardware needs a separate bit for describing the direction (positive = 1, negative = 0). If you have read-only sections with segmentation, you can even share them between 
similar processes (such as the code section). But you'd need to store three extra bits to signal read-write-execute priviliges in the hardware.

This is an example of a coarse-grained segmentation. A fine-grained segmentation would require several smaller-sized and segments which would need more resources from the hardware (such as a segment table).

Unfortunately, there is still another major issue with segmentation; external fragmentation, i.e that memory between segments 
are poorly utilized after a while. There are ways to manage this (e.g by good process-creation algorithms to prevent external
fragmentation, or by compacting main memory, i.e reorganize it, in order to fix the external fragmentation). We will cover the 
prevention next.

## 
