#include "OneBin.h"
#include <stdio.h>
#include <stdlib.h>

FreeList freelist = NULL;

// function to initialize our One Bin allocator
int init_allocator(int binSize){

  // compute the num of chunks that will be present in total
  unsigned long num_chunks = TOTAL_SIZE/binSize;

  // allocate memory for the free list
  freelist = malloc(sizeof(struct freeList));
  if(!freelist){
    printf("\nError in init_allocator(): Unable to allocate memory for free list using malloc.\n");
    return 0;
  }

  // allocate the first bin
  FreeBin freebin = malloc(binSize);
  if(!freebin){
    printf("\nError in init_allocator(): Unable to allocate memory for a bin using malloc.\n");
    return 0;
  }
  //add the newly created bin to the free list
  freebin->next = NULL;
  freelist->head = freebin;

  // create the remaining free bins and add them to the free list
  for(int i=1; i<num_chunks; i++){
    freebin = malloc(binSize);
    if(!freebin){
      printf("\nError in init_allocator(): Unable to allocate memory for a bin using malloc.\n");
      return 0;
    }
    freebin->next = freelist->head;
    freelist->head = freebin;
  }

  return 1;
}


// function to allocate memory of one bin size
void* ob_malloc(){

  //validate inputs
  if(!freelist){
    printf("\nError in ob_malloc(): Allocator not initialized.\n");
    return NULL;
  }

  // no more memory to allocate
  if(!freelist->head){
    printf("\nError in ob_malloc(): Free list is empty.\n");
    return NULL;
  }

  // get one of the bins from the free list and return it for the user
  FreeBin freebin = freelist->head;
  freelist->head = freebin->next;

  return freebin;
}

// function to free a one bin allocated bin
void ob_free(void* ptr){

  // validate input
  if(!ptr){
    printf("\nError in ob_free(): NULL passed in as input parameter.\n");
    return;
  }

  // convert the pointer to a free bin
  FreeBin freebin = ptr;

  // add it to the free list
  freebin->next = freelist->head;
  freelist->head = freebin;

  return;
}
