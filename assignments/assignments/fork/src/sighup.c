#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void handler(int sig) {
  printf("received signal %d\n", sig);
}



int main() {


  struct sigaction sa;
  
  int pid = fork();

  /* this is the simple version of signal handler */
  sa.sa_handler = handler;

  /* clear the mask i.e. do not block any signals while in the handler */
  sigemptyset(&sa.sa_mask);

  if(sigaction(SIGHUP, &sa, NULL) != 0) {
    fprintf(stderr, "something happend\n");
    return(1);
  }

  
  if(pid == 0) {
    int child = getpid();

    printf("child(%d): parent %d, group %d, session %d\n", getpid(),  getppid(), getpgid(child), getsid(child));
    sleep(10);
    printf("child(%d): parent %d, group %d, session %d\n", getpid(), getppid(), getpgid(child), getsid(child));
    sleep(10);
    printf("child(%d): parent %d, group %d, session %d\n", getpid(), getppid(), getpgid(child), getsid(child));    
  } else {
    int parent = getpid();
    printf("parent(%d): parent %d, group %d, session %d\n", getpid(), getppid(), getpgid(parent), getsid(parent));    
    sleep(5);
    int zero = 0;
    int i = 3 / zero;
  }
  return 0;
}
