#ifndef _INITNU_H
#define _INITNU_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct sortThread {
  int threadnu;
  int col;
  int * s;
  int id;
};
int initnu(struct sortThread** sort); 
#endif // !
