#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "strtostr.h"
int randwfile(const int src_file ,const int dest_file) {

    unsigned char buff[100];
    unsigned char tempbuff[100];
    int fileNowOffset = 0;   
    while(read(src_file , buff , sizeof(buff)) > 0) {
       int newOffset =  0; 
       int lastOffset = 0;
       int nowOffset = 0;
       int nowSrcOffset = 0;

     lastOffset = newOffset;
      
    while(buff[++newOffset] != '\n'){

    }

    newOffset++;
    fileNowOffset += newOffset;
    lseek(src_file , fileNowOffset ,SEEK_SET);
    for(int i = 0 ; i < newOffset - lastOffset  ; i++) {
        tempbuff[nowOffset++] = buff[i + lastOffset]; 
        // printf("%c" ,tempbuff[i]);
    }

    // printf("strtostr over\n"); 
    write(dest_file , tempbuff , strtostr(tempbuff , newOffset - lastOffset));

  }
  return 0;

}
