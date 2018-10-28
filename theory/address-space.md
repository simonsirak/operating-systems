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
