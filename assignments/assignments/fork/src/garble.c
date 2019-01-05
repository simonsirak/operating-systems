#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  int pid;
  
  int fd = open("garble.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

  if(fd == -1) {
    printf("failed to open file\n");
    return 0;
  }
    
  pid = fork();
  
  if(pid == 0) {
    dprintf(fd, "this is ");
    sleep(1);
    dprintf(fd, "the child ");
    sleep(1);
    dprintf(fd, "printing ");          
    close(fd);
  } else {
    sleep(1);
    dprintf(fd, " -mother- ");
    sleep(1);
    dprintf(fd, " -mother- ");      
    close(fd);
  }

  return 0;
}
