# What is a Thread?
A thread is pretty much a component of the process that has its own PC and its own stack. The threads of a program are what make up the 
entire process, and the threads all share the address space of that process. 

This sharing adds issues regarding concurrency, such as data races/race conditions (producing indeterminate programs) and deadlocks. We will discuss these in due time.

A thread is stored as a Thread Control Block, TCB -- essentially a thread-correspondent structure to the Process Control Block, PCB. 
Switching between threads needs to use the TCB as well. 

# Why Threads?
We use threads for their parallelism. They bring parallelism within a process, just like context-switches bring parallelism between processes. 
It also allows you to switch to another thread of the *same* process, despite the currently running sibling-thread gets blocked. 

# Atomicity: How to Handle Threads
In order to take care of race conditions, you will need some sort of atomicity. An atomic procedure is one that ensures that it is not 
interrupted midway, and is very important for applications such as databases. It is also important in other "multi-accessible" areas such 
as threads. Thus, a programmer needs tools for creating their own atomic procedures. These will be called synchronization primitives.
