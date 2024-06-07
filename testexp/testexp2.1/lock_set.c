#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int copy_str(const char* talk , unsigned char *tempbuff ,int* tempbuffpoint) {
  char* str;
  printf("%s" , talk);
  scanf("%s" , str);
  for(int i = 0 ; i < strlen(str) ; i++) 
  {
    tempbuff[(*tempbuffpoint)++] = str[i];
  }
  tempbuff[(*tempbuffpoint)++] = ' ';
  tempbuff[(*tempbuffpoint)++] = ' ';


}
int lock_set(int fd , int type) {
  int  id ,no[5] , nop = 0 , tbp = 0 , offset = 0;
  int real_read_len;
  char strToi[10];
  unsigned char buff[1024];
  unsigned char tempbuff[1024];
  struct flock lock;


    while((real_read_len = read(fd , buff , sizeof(buff))) > 0)
  {
    for( int i = 0; i < real_read_len ; i++) {
      printf("%c" , buff[i]);
      if(buff[i] == '\n')
        no[++nop] = i + 1;
    }
  }
  printf("请输入你要修改的编号\n");
  scanf("%d", &id);
  if(id > nop) 
  {
    printf("没有这个编号哦");
    exit(EXIT_FAILURE);
  }
  lock.l_whence = SEEK_SET;
  lock.l_start = no[id - 1];
  lock.l_len = no[id] - no[id - 1];
  lock.l_type = type;
  lock.l_pid = -1;


  fcntl(fd , F_GETLK , &lock);
  if(lock.l_type != F_UNLCK) {
    if(lock.l_type == F_RDLCK) {
      printf("Read lock already set by %d\n" , lock.l_pid);
    }
    else if(lock.l_type == F_WRLCK) {
      printf("Write lock already set by %d\n" , lock.l_pid);
    }
  }
  lock.l_type = type;
  if((fcntl(fd , F_SETLKW ,&lock)) < 0)
  {
    printf("Lock failed : type = %d\n" , lock.l_type);
    return 1;
  }

  switch(lock.l_type) {
    case F_RDLCK:
      {
        printf("Read lock set by %d\n" , getpid());
      }
      break;
    case F_WRLCK:
      {
        for(int i = 0 ; i < no[id -1]; i++) {
          tempbuff[tbp++] = buff[i];
        }

        printf("Write lock set by %d\n" , getpid());
        sprintf(strToi, "%d" , id);
        tempbuff[tbp++] =strToi[0];
        tempbuff[tbp++] = ' ';
        tempbuff[tbp++] = ' ';

        copy_str("请输入姓名" , tempbuff , &tbp);
        copy_str("请输入手机号" , tempbuff , &tbp);
        copy_str("请输入固定电话号" , tempbuff , &tbp);
        copy_str("请输入地址" , tempbuff , &tbp);
       // for( int i = 0 ; i < nop ; i++) {
       //    if(i == id)
       //      if(i == 1){
       //        offset = tbp - no[i];
       //        no[i] = tbp; 
       //      }
       //      else {
       //        offset = tbp -(no[i] - no[i - 1]);
       //        no[i] = no[i -1] + tbp;
       //      }
       //    else if (i > id){
       //      no[i] = no[i] + offset;
       //    }
       // }
        for(int i = no[id] ; i < no[nop]; i++) {
          tempbuff[tbp++] = buff[i];
        }
        write(fd , tempbuff , tbp);

      }
      break;
    case F_UNLCK:
      {
        printf("Release lcok by %d" , getpid());
        return 1;
      }
      break;
    default:
      break;
  }
 
  

  return 0;
}
