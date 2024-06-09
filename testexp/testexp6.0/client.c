#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>


#define BUFFER_SIZE 1024

int main(int argc, char *argv[])
{
  int fifo_fd;
  char buffer[BUFFER_SIZE];
  
  if(argc != 2) {
    printf("Usage:%s filename" , argv[0]);
    exit(EXIT_FAILURE);
  }
  if(access(argv[1] , F_OK) == -1) {
    if(mkfifo(argv[1] , 0666)== -1 ) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  fifo_fd = open(argv[1] , O_RDWR);
  if(fifo_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }
  snprintf(buffer , BUFFER_SIZE , "Client request");
  if(write(fifo_fd , buffer , BUFFER_SIZE) == -1) {
    perror("write");
    exit(EXIT_FAILURE);
  }
  printf("Cilent: Sent request to server\n");


  if(read(fifo_fd , buffer , BUFFER_SIZE) > 0) {
    printf("Cilent : Received unique number from server :%s \n" , buffer);
  } else {
    perror("read");
  }
  close(fifo_fd);
  return EXIT_SUCCESS;
}
