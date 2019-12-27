#include "SimulateBuddyHelper.h"
#include "WeightedBuddy.h"
#include <stdio.h>
#include <stdlib.h>

// functions to create block
Block createBlock(void *ptr){

  //validate input
  if(!ptr){
    printf("\nError in createBlock(): Invalid input.\n");
    return NULL;
  }

  // allocate memory for a block
  Block block = (Block) malloc(sizeof(struct block));
  if(!block){
    printf("\nError in createBlock(): Unable to allocate mem.\n");
    return NULL;
  }

  // set the fields
  block->ptr = ptr;
  block->next = NULL;

  return block;
}

// functions to create list
TimeList createTimeList(){

  // allocate memory for the time list
  TimeList tl = (TimeList) malloc(sizeof(struct timeList));
  if(!tl){
    printf("\nError in createTimeList(): Unable to allocate mem.\n");
    return NULL;
  }

  tl->head = NULL;

  return tl;
}

// function to insert a block into a particular time list
int insertBlock(TimeList tl, void*ptr, int n){

  //validate inputs
  if(!tl || !ptr){
    printf("\nError in insertBlock(): Invalid input.\n");
    return 0;
  }

  // create a block
  Block block = createBlock(ptr);
  if(!block){
    printf("\nError in insertBlock(): Unable to create a bock.\n");
    return 0;
  }

  // add the block to the list
  if(!tl->head){
    tl->head = block;
  }
  else{
    block->next = tl->head;
    tl->head = block;
  }

  return 1;
}

// function to free all the blocks scheduled at the time
int freeBlocksTimeList(TimeList tl, int n){

  //validate input
  if(!tl){
    printf("\nError in freeBlocksTimeList(): Invalid input.\n");
    return 0;
  }

  // get the block pointer to iterate the list
  Block block = tl->head;
  Block temp = block;

  // free the list and the blocks too
  while(block){
    SP_free(block->ptr);
    block = block->next;
    free(temp);
    temp = block;
  }

  return 1;
}
