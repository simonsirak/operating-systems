The POSIX Thread API is a good example of how a thread API should look.

# Creating Threads
You create threads using `pthread_create`. This function mainly requires you to pass a function pointer (i.e a pointer to where the thread 
should start its execution, essentially like "the main function" of that thread), the address of a variable to store a handle of the
created thread, and optionally a bunch of arguments wrapped in a void pointer.
