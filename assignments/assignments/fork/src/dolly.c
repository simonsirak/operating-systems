#include <stdio.h>
#include <unistd.h>

int main() {

  int pid = fork();

  if(pid == 0) {
    printf("I'm the child process and my pid is %d\n", getpid());
    sleep(1);
  } else {
    printf("I'm the mother process and my child is called %d\n", pid);
  }
  printf("This is the end (%d)\n", getpid());

  return 0;
}
