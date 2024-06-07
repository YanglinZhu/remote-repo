#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
  pid_t child;
  child = fork();
  if(child == -1) {
    printf("子进程创建错误！");
    exit(EXIT_FAILURE);
  }

  else if(child == 0) {

    if(execlp("mplayer" , "mplayer" ,
          "/home/Zyl/Downloads/test.mp4", NULL) < 0) {
     printf("Child execl error\n");
     exit(EXIT_FAILURE);
    }
    sleep(10);
  }
  else {
    printf("father progress over");
  }

  return EXIT_SUCCESS;
}
