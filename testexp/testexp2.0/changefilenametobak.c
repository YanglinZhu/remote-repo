#include <string.h>
#include <stdio.h>
// 这是一个将一个字符串截取某一字符前的所有字符并在其之后添加特定字符串
int changefilenametobak(const char* src_filename ,char* dest_filename,const char divisionpoint ,const char* appendstr){
  char Dest_file_Name[100];
  int i = 0;
  if(sizeof(src_filename) > sizeof(Dest_file_Name)) {
    printf("输入的名字太长啦 , 失败啦");
    return -1;
  }
  char* position = strchr(src_filename , divisionpoint);
  if(position != NULL) {
  size_t length = position - src_filename;
  strncpy(Dest_file_Name , src_filename , length);
  Dest_file_Name[length] = '\0';
  strncat(Dest_file_Name , appendstr , sizeof(appendstr) - 1);
  }else {
    strcpy(Dest_file_Name , src_filename);
    strncat(Dest_file_Name , appendstr , sizeof(appendstr) - 1);
  }
  for(; Dest_file_Name[i] != '\0' ; i++) {
    dest_filename[i] = Dest_file_Name[i];
  }
  dest_filename[i] = '\0';
  return 0;
} 
