#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "initnu.h"
int compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
 
    // return (arg1 > arg2) - (arg1 < arg2); // 可行的简写
    // return arg1 - arg2; // 错误的简写：整数溢出时为未定义行为，比如此处使用 int_MIN 时
}
void merge_sorted_arrays(int* s1, int col1, int* s2, int col2, int* result, int totalnu) {
    int i = 0, j = 0, k = 0;
    while (i < col1 && j < col2) {
        if (s1[i] <= s2[j]) {
            result[k++] = s1[i++];
        } else {
            result[k++] = s2[j++];
        }
    }
    while (i < col1) {
        result[k++] = s1[i++];
    }
    while (j < col2) {
        result[k++] = s2[j++];
    }
}

void* Sort(void* inputSort) {
  struct sortThread* sort = (struct sortThread*)inputSort;
  qsort(sort->s, sort->col, sizeof(int), compare_ints);
  return(void*)0;
}

int main(int argc, char *argv[])
{
  pthread_t threads[4];
  int *result; 
  int totalnu = 0;
  int threadnu = 0;
  printf("请输入线程数:\n");
  scanf("%d" , &threadnu);

  struct sortThread* sort[threadnu];
   for(int i = 0 ; i < threadnu ; i++) {
   sort[i] = (struct sortThread *)malloc(sizeof(struct sortThread)*threadnu);
  }
  for(int i = 0 ; i < threadnu ; i++) {
    sort[i]->threadnu = threadnu;
  }
  

  int choice = 0;
  printf("是否统一每一个线程处理的数据量(1.是 2.否)");
  scanf("%d" , &choice);
  if(choice == 2)
    for(int i = 0 ; i < threadnu ; i++) {
    printf("请输入%d号线程处理数据量:\n" , i);
    scanf("%d" , &sort[i]->col); 
    }
  else if(choice == 1) {
      printf("请输入线程处理数据量:\n");
      scanf("%d" , &sort[0]->col); 
      for(int i = 1 ; i < threadnu ; i++ ) 
      sort[i]->col = sort[0]->col;
  }

  for(int i = 0 ; i < threadnu ; i++ ) {
   sort[i]->s = (int *)malloc(sort[i]->col*sizeof(int));
  }

  initnu(sort);

  for (int i = 0; i < threadnu; i++) {
    totalnu += sort[i]->col;
    int result = pthread_create(&threads[i], NULL, Sort ,(void *)sort[i]);
        if (result != 0) {
            printf("Error creating thread %d", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < threadnu; i++) {
        pthread_join(threads[i], NULL);
        
    }
    result = (int *)malloc(sizeof(int)*totalnu); 
    int * tempResult = (int *)malloc(sizeof(int)*(sort[0]->col + sort[1]->col));
    if(threadnu > 2)
        for( int i = 0 , k = 0 ; i < threadnu - 1 ; i ++) {
          if(i == 0)
          {
            merge_sorted_arrays(sort[i]->s ,sort[i]->col ,sort[i + 1]->s,
                sort[i + 1]->col , tempResult ,sort[i]->col + sort[i + 1]->col);
            k = sort[i]->col + sort[i + 1]->col;
          }
          else {
            int * tResult = (int*)malloc(sizeof(int)*(k + sort[i + 1]->col));
            merge_sorted_arrays(tempResult , k , sort[i + 1]->s , 
                sort[i + 1]->col , tResult , k + sort[i + 1]->col);
            k += sort[i + 1]->col;
            free(tempResult);
            tempResult = (int *)malloc(sizeof(int)*k);
            memcpy(tempResult , tResult , k*sizeof(int));
            free(tResult);
          }
          if(i == threadnu - 2) {
            memcpy(result , tempResult , totalnu*sizeof(int));
          }
        }
    else if(threadnu == 1){
      memcpy(result , sort[0]->s , totalnu*sizeof(int));
    }
    else if(threadnu == 2) {
      merge_sorted_arrays(sort[0]->s ,sort[0]->col ,sort[1]->s,
                sort[1]->col , result ,sort[0]->col + sort[1]->col);
    }
    for(int i = 0 ; i < totalnu; i++) {
      printf("%d " , result[i]);
    }

    for(int i = 0 ; i < threadnu ; i++) 
    free(sort[i]);
  return EXIT_SUCCESS;
}
