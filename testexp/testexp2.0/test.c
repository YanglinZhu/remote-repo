
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include "changefilenametobak.h"
#include "RandWfile.h"
int main(int argc, char *argv[])
{

  int src_file ,dest_file;
  char Dest_file_Name[100];
  if(argc < 2) {
    printf("Usage : %s filename\n" , argv[0]);
  }
  src_file = open(argv[1] , O_RDONLY);
  changefilenametobak(argv[1] , Dest_file_Name, '.' , "_bak.c");

  dest_file = open(Dest_file_Name , O_WRONLY|O_CREAT , S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);
  if(src_file < 0 || dest_file < 0) {
      printf("你要打开的文件错误");
      exit(EXIT_FAILURE);
  }

  randwfile(src_file , dest_file);
  close(dest_file);
  close(src_file);
  return EXIT_SUCCESS;
}
