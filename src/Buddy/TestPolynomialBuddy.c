#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Polynomials.h"
#include "WeightedBuddy.h"

// function to print the simulation results
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

  return;
}

int main(){

  if(!init_SPHeap()){
    printf("\nError in main(): Unable to initialize SP head.\n");
    exit(0);
  }

  //input the divident polynomial
  printf("\nEnter the dividend polynomial.");
  Polynomial dividendPolynomial = takeInputPolynomial();
  if(!dividendPolynomial){
    printf("\nFailed to take input.\n");
    return 0;
  }

  //input the divisor polynomial
  printf("\nEnter the divisor polynomial.");
  Polynomial divisorPolynomial = takeInputPolynomial();
  if(!divisorPolynomial){
    printf("\nFailed to take input.\n");
    return 0;
  }

  int n=1000;

  while(n){

    n--;

    //create empty polynomials to store the result
    Polynomial quotientPolynomial = createPolynomial();
    if(!quotientPolynomial){
      printf("\nFailed to take input.\n");
      return 0;
    }
    Polynomial remainderPolynomial = createPolynomial();
    if(!remainderPolynomial){
      printf("\nFailed to take input.\n");
      return 0;
    }

    //perform the division
    if(!dividePolynomials(dividendPolynomial, divisorPolynomial, quotientPolynomial, remainderPolynomial)){
      printf("\nFailed to divide polynomials.\n");
      return 0;
    }

    //free all the polynomials we used
    freePolynomial(remainderPolynomial);
    freePolynomial(quotientPolynomial);

  }

  //un-comment to get the instrumentation of SP Heap allocator
  //printData();

  double timeTaken = getTime();
  printf("\nTime taken for memory management in the program by Weighted Buddy allocator: %lf\n", timeTaken);

  printf("\nEnding the program....\n");


  return 0;
}
