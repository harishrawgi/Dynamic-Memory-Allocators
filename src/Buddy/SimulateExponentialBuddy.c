#include "WeightedBuddy.h"
#include "SimulateBuddyHelper.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define UPPER 2000
#define LOWER 100
#define MAX_TIME 1000

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

// function to perform the simulation
int testExponentialDistribution(){

  //set the initial time
  int time=0;

  // create an array of TimeList for storing all the blocks that have been scheduled for de-allocation acc to time
  TimeList tl[MAX_TIME];
  for(int i=0; i<MAX_TIME; i++){
    tl[i] = createTimeList();
    if(!tl[i]){
      printf("\nError in testExponentialDistribution(): Unable to create a time list.\n");
      return 0;
    }
  }

  // read the exponentially distributed random numbers from the file (generated using python)
  FILE * fp;
  fp = fopen("exp.txt", "r");
  if (fp == NULL) {
    printf("failed to open file\n");
    return 1;
  }

  float fval[MAX_TIME];
  int n, i;
  n = 0;
  while (fscanf(fp, "%f", &fval[n++]) != EOF);

  // convert all the real numbers to integers
  int sval[MAX_TIME];
  for(int i=0; i<MAX_TIME; i++){
    sval[i] = (int) fval[i];
  }

  // perform the simulations
  while(time < MAX_TIME - 1){

    time++;

    // free all the blocks scheduled
    freeBlocksTimeList(tl[time], time);

    int s = sval[time-1];
    int t = (rand() % (100 - 1 + 1)) + 1;

    int lifetime = time + t;

    if(lifetime > 999){
      continue;
    }

    void* x = SP_malloc(s);
    if(!insertBlock(tl[lifetime], x, lifetime)){
      printf("\nError in testExponentialDistribution(): Unable to insert block in time list.\n");
      return 0;
    }

  }

  printData();

  return 1;
}

int main(){

  // seed the random number generator
  srand(time(0));

  // initialize the SP heap
  if(!init_SPHeap()){
    printf("\nError in main(): Unable to initialize SP head.\n");
    exit(0);
  }

  // perform the simulation test
  if(!testExponentialDistribution()){
    printf("\nError in main(): testExponentialDistribution() failed.\n");
    exit(0);
  }

  return 0;
}
