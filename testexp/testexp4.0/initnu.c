#include "initnu.h"
int initnu(struct sortThread** sort) {
  srand(time(NULL));
  for(int j = 0 ; j < sort[0]->threadnu ; j++)
    for(int i = 0 ; i < sort[j]->col ; i++) {
      sort[j]->s[i] = rand();
    }
  return 0;
}
