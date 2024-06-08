#include "sigalrm.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int ret;
  ret = set_signal_handler();
  if(ret != 0) {
    printf("set_signal_handler error\n");
  }
  while(1) {
  }
  return EXIT_SUCCESS;
}
