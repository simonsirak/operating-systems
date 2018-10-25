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
