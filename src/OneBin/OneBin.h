#ifndef ONEBIN_H
#define ONEBIN_H

#define TOTAL_SIZE 256*1024*1024

typedef struct freeBin {
  struct freebin* next;
}* FreeBin;

typedef struct freeList{
  struct freebin* head;
}* FreeList;

// function to initialize our One Bin allocator
int init_allocator(int binSize);

// function to allocate memory of one bin size
void* ob_malloc();

// function to free a one bin allocated bin
void ob_free(void* ptr);

#endif
