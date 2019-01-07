# Unix Process API
The three core process API calls in UNIX are called `fork()`, `wait()/waitpid()` and `exec()`. These are all accessible
in C by including the library `<unistd.h>`.

Forking simply means "copying" the current process into a child process, including its PC state and everything. 
It also copies file descriptors. Note that it copies them, so if you close a file descriptor from a child, it 
only closes the file descriptor for the child. Fork returns 0 for the child, and the child's ID is returned for the parent.

Waiting simply means "waiting" for ALL child processes to terminate. Wait then returns 0. If you give a PID as input (e.g for waitpid()),
then it waits for that and returns the exit code of that process.

Executing simply means "execute" some executable, given some arguments. This transforms the current process into a process for the 
executable. Therefore, a call to `exec` never returns. There are some variants of exec in different Linux-based OS:s.

This API makes it a lot easier for the shell to operate.
