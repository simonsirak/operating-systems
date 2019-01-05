#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  int pid;
  pid = fork();
  
  if(pid == 0) {
    int fd = open("over.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd != -1) {
      dprintf(fd, "this is the child\n");
      close(fd);
    }
  } else {
    int fd = open("over.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if(fd != -1) {
      wait(NULL);
      dprintf(fd, "over write\n");
      close(fd);
    }
  }

  return 0;
}
