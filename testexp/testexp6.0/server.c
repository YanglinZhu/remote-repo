#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024

int main(int argc , char* argv[]) {
  if(argc != 2){
    printf("Usage: %s filename" , argv[0]);
    exit(EXIT_FAILURE);
  }
  int fifo_fd;
  char buffer[BUFFER_SIZE];
  int unique_number = 0;
  int ret = access(argv[1]  , F_OK);
  if(ret == -1) {
    if(mkfifo(argv[1] , 0666) == -1) {
      perror("mkfifo");
      exit(EXIT_FAILURE);
    }
  }
  printf("Server :Waiting for Clients...\n");


  fifo_fd = open(argv[1] , O_RDWR);
  if(fifo_fd == -1) {
    perror("open");
    exit(EXIT_FAILURE);
  }

  while(1) {
    if(read(fifo_fd , buffer , BUFFER_SIZE) > 0) {
      printf("Server : Received request form client: %s \n" , buffer);

      snprintf(buffer , BUFFER_SIZE , "%d" , unique_number ++);

      if(write(fifo_fd , buffer ,BUFFER_SIZE) == -1) {
        perror("write");
        exit(EXIT_FAILURE);
      }
      printf("Server :Sent unique number %s to clinet \n" , buffer);
    }
  }
  close(fifo_fd);
  // unlink(argv[1]);
}
