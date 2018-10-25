# Process
A process is an abstraction of a running program. It contains metadata regarding the execution of a program, 
that is relevant to the OS. The abstraction allows the OS to perform time-sharing based scheduling to share CPU 
resources. A process usually has the following state:

* An *address space*. This is used to manage its main memory (which is part of the process machine state).
* A program counter, *PC* (aka IP, instruction pointer).
* A stack pointer,
* A frame pointer,
* I/O information (such as which files are open).

A process usually has the following API:

* Create (create a new process)
* Destroy (kill a process prematurely)
* Wait (force process to wait for some other process)
* Status (show status of process such as the state and running time).

A process is created by loading code and static variables to the process address space, allocating stack and heap-space and opening up the three special file descriptors stdin, stdout, stderr.

The three overall states of a process are *ready*, *running*, *blocked*. Blocked is usually used when I/O is performed on the process, allowing other processes to work while the I/O is done. Ready and running are interchanged when scheduling the process. There are sometimes initial and final states as well. For instance, the *zombie* state is a final state in UNIX-systems in which the process isn't removed but it has "stopped". It can be used by other processes to read the return code before the process is destroyed.
