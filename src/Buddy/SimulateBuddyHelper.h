#ifndef SIMULATEBUDDYHELPER_H
#define SIMULATEBUDDYHELPER_H

// structure for a block of memory
typedef struct block{
  void* ptr;
  void* next;
}* Block;

// list which stores all the blocks scheduled to be de-allocated at the same time
typedef struct timeList{
  Block head;
}* TimeList;

// functions to create block
Block createBlock(void *ptr);

// functions to create list
TimeList createTimeList();

// function to insert a block into a particular time list
int insertBlock(TimeList tl, void*ptr, int n);

// function to free all the blocks scheduled at the time
int freeBlocksTimeList(TimeList tl, int n);

#endif
