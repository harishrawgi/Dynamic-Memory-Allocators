#include "OneBin.h"
#include <stdio.h>
#include <stdlib.h>

int main(){

  if(!init_allocator(sizeof(double))){
    printf("\nError in main(): Unable to initialize OneBin allocator.\n");
    exit(0);
  }

  int* x = (int*) ob_malloc();
  *x = 5;

  printf("\nx is: %d\n", *x);
  printf("\nThe size of the allocated memory for x is: %d\n", sizeof(*x));

  double* y = (double*) ob_malloc();
  *y = 6.78;

  printf("\ny is: %lf\n", *y);
  printf("\nThe size of the allocated memory for y is: %d\n", sizeof(*y));

  return 0;
}
