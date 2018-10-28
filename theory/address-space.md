# Address Space
An address space is an abstraction that lets each process have its own virtual addresses that get translated 
to physical addresses in main memory. This translation is usually done with the help of hardware, in which it is then 
called dynamic translation. The abstraction came from the need of implementing time-sharing (e.g context switches) while 
not having to load processes from disk every context switch. The concept of an address space is thus to tackle the problem of having protection between processes
while maintaining efficiency (time and spacewise). How are address spaces usually implemented? Below are increasingly complex
additions to construct the abstraction.

