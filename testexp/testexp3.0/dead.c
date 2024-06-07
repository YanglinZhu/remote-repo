#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  pid_t child;
  child = fork();
  if(child == -1) {
    printf("child fork error\n");
    exit(EXIT_FAILURE);
  }
  else if (child == 0) {
    printf("child pid %d\n" , getpid());
  }
  else {
    printf("farther pid %d\n" , getpid());

    while(1) {
      sleep(10);
    }
  }
  return 0;
}
