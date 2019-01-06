#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
    int fd = open("quotes.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

    int pid = fork(); // all descriptors are shared between child and parent process, i.e copies of file descriptors are made.

    if(pid == 0){
        dup2(fd, 1); // duplicates file descriptor fd to stdout of this process.
        // stdout is "closed" due to this, and replaced with a copy of fd.
        close(fd); // child closes its original copied file descriptor (the copy on 1 still exists)
        execl("boba", "boba", NULL); // modify this program to become boba. 1 is still a copy of fd, even though fd was closed above. Exec* preserve file descriptors
    } else {
        //waitpid(pid, NULL, 0); // parent waits for child to finish
        dprintf(fd, "Arghhhh"); // mother process writes this to fd. 
        close(fd); // mother process closes this fd.

        // is printf atomic tho? As in, will it execute entirely?
    }
}
