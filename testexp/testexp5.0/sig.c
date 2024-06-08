#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "sig.h"
void do_Sigint() {
  static int intno = 0;
  printf("You pressed Ctrl+C\n");
  if(++intno==9) {
  printf("Ctrl+C already pressed %d times\n" , intno);
    exit(EXIT_SUCCESS);
  }
  printf("Ctrl+C already pressed %d times\n" , intno);
}
void do_sigout() {
  static int outno = 0;
  printf("You pressed Ctrl+\\\n");
  if(++outno==5) {
  printf("Ctrl+\\ already pressed %d times\n" , outno);
    exit(EXIT_SUCCESS);
  }
  printf("Ctrl+\\ already pressed %d times\n" , outno);
}
void process_signal(int signo) {
  if(signo == SIGINT) {
    do_Sigint();
  }
  else if(signo == SIGQUIT) {
    do_sigout();
  }
}

int set_signal_handler() {
  struct sigaction act , oldact;
  act.sa_handler = process_signal;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;


  if(sigaction(SIGINT , &act , &oldact) == -1) {
    perror("can not catch SIGINT");
    return -1;
  }


  if(sigaction(SIGQUIT , &act , &oldact) == -1) {
    perror("can not catch SIGQUIT");
    return -1;
  }

  return 0;
}
