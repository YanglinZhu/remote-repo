#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "sigalrm.h"


void do_signal_alarm(int signo) {
  static int alarmtimes = 0;
  if(signo == SIGALRM) {
    if(++alarmtimes == 10){
    printf("第%d次:Hello\n" , alarmtimes);
      exit(EXIT_SUCCESS);
    }
    printf("第%d次:Hello\n" , alarmtimes);
  }
}

int set_signal_handler() {
  struct itimerval new_time , old_time;
  struct timeval timep , timeu;
  timep.tv_sec = 2;
  timep.tv_usec = 0;
  timeu.tv_sec = 2;
  timeu.tv_usec = 0;
  new_time.it_interval = timep;
  new_time.it_value = timeu;

  struct sigaction act , oldact;
  act.sa_handler = do_signal_alarm;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  setitimer(ITIMER_REAL , &new_time , &old_time);

  if(sigaction(SIGALRM , &act , &oldact) == -1) {
    perror("can not catch SIGINT\n");
    return -1;
  }
  return 0;
}
