#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "lock_set.h"
int main(int argc , char* argv[]) {
  int fd;
  int real_read_len;
  fd = open("contack.data" , O_RDWR|O_APPEND );
  if(fd < 0)
  {
    printf("Open file error\n");
    exit(EXIT_FAILURE);
  }
  lock_set(fd , F_WRLCK);
 
}
