#include "WeightedBuddy.h"
#include <stdio.h>
#include <stdlib.h>

#define MB 1024*1024

void printData(){
  double internalFragmentation = getInternalFragmentation();
  printf("\nThe current internal fragmentation is: %lf\n", internalFragmentation);

  long num_splits = getNumSplits();
  printf("\nThe current number of splits is: %ld\n", num_splits);

  long num_merges = getNumMerges();
  printf("\nThe current number of merges is: %ld\n", num_merges);

  long num_alloc_requests = getNumAllocRequests();
  printf("\nThe current number of allocation requests is: %ld\n", num_alloc_requests);

  long num_deallocation_requests = getNumDeallocRequests();
  printf("\nThe current number of de-allocation requests is: %ld\n", num_deallocation_requests);
}

int main(){

  if(!init_SPHeap()){
    printf("\nError in main(): Unable to initialize SP head.\n");
    exit(0);
  }

  void* x_128MB = SP_malloc(128*MB);
  printf("\nThe size of the allocated memory for x_128MB is: %ld\n", sizeof(*x_128MB));
  printf("The address of x_128MB is: %p", x_128MB);

  void* x_64MB = SP_malloc(64*MB);
  printf("\nThe size of the allocated memory for x_64MB is: %ld\n", sizeof(*x_64MB));
  printf("The address of x_64MB is: %p", x_64MB);

  void* x_32MB = SP_malloc(32*MB);
  printf("\nThe size of the allocated memory for x_32MB is: %ld\n", sizeof(*x_32MB));
  printf("The address of x_32MB is: %p", x_32MB);

  void* x2_64MB = SP_malloc(64*MB);
  if(!x2_64MB){
    printf("\nMessage from main(): Memory can't be alloc for x2_64MB\n");
  }
  else{
    printf("\nThe size of the allocated memory for x_64MB is: %ld\n", sizeof(*x_64MB));
    printf("The address of x_64MB is: %p", x_64MB);
  }

  SP_free(x_32MB);

  x2_64MB = SP_malloc(64*MB);
  if(!x2_64MB){
    printf("\nMessage from main(): Memory can't be alloc for x2_64MB\n");
  }
  else{
    printf("\nThe size of the allocated memory for x2_64MB is: %ld\n", sizeof(*x2_64MB));
    printf("The address of x2_64MB is: %p", x2_64MB);    
  }


  printData();


  /*
  int* x = (int*) SP_malloc(sizeof(int));
  *x = 5;

  printf("\nx is: %d\n", *x);
  printf("\nThe size of the allocated memory for x is: %d\n", sizeof(*x));

  printf("\nThe address of x is: %p", x);

  SP_free(x);

  double* y = (double*) SP_malloc(sizeof(double));
  *y = 6.78;

  void* p = SP_malloc(9);

  printf("\ny is: %lf\n", *y);
  printf("\nThe size of the allocated memory for y is: %d\n", sizeof(*y));

  */

  return 0;
}
