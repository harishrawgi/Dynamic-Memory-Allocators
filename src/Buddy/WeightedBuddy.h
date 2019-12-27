#ifndef WEIGHTEDBUDDY_H
#define WEIGHTEDBUDDY_H

// structure defining a node in the ASL list
typedef struct aslNode{
  struct aslNode* next;
  struct aslNode* prev;
  void* ptr;
} * ASLNode;

typedef struct asl{
  struct aslNode* head;
  struct aslNode* tail;
} * ASL;



// defines the maximum allocation possible, i.e. 256 MB (2^28 Bytes)
#define MAX_ALLOCATION_BITS 28
#define MAX_ALLOCATION_OFFSET ((size_t)1 << MAX_ALLOCATION_BITS)

// defines the minimum allocation possible
//#define MIN_ALLOCATION_BITS 4
//#define MAX_ALLOCATION_OFFSET ((size_t)1 << MIN_ALLOCATION_BITS)
#define MIN_ALLOCATION_BITS 0
#define MIN_ALLOCATION_OFFSET ((size_t)1 << MIN_ALLOCATION_BITS)

// defines the number of possible block sizes
#define SIZES 2*M //+ 1

// defines the value of m
#define M 28

// defines the total number of unique addresses in the space
#define NUM_NODES 256*1024*1024

// define the TAG, TYPE and KVAL headers for getting their values for indo bytes
#define TAG_CHECK 1
#define TYPE_CHECK 6
#define KVAL_CHECK 248

// function to create a new node
void* createAslNode();

// function to get index from address for a node
int getIndexFromAddress(void* ptr, void* bptr);

// function to set the info byte of a node of given index
int setInfo(int index, int kval, int type, int tag);

// function to set the info byte of a node of given index
int setTag(int index, int tag);

// function to get teh ASL index depending on the size of the node
int getIndexForListFromSize(int size);

// add the node to the ASL
int insertIntoAsl(ASL list, ASLNode node);

// function to get a node from ASL for a given index
ASLNode getNodeFromList(int list_index, void* bptr);

// function to split a node to two
int splitNode(int split_index, int* left_index, int* right_index, void* bptr);

// function to search for the address and remove it from the list
ASLNode removeNodeFromAsl(ASL list, void* address);

// function to compute the index of buddy for the info array based on the address and index of the node
int getBuddyIndex(void* address, int index);

// getter functions for the various analysis data
double getInternalFragmentation();
long getNumSplits();
long getNumMerges();
long getNumAllocRequests();
long getNumDeallocRequests();

// function to initialize the SP heap memory
int init_SPHeap();

// function to allocate memory as per the supplied size
void* SP_malloc(int size);

// function to deallocate memory for the supplied address
void SP_free(void* address);

// function to perform the merging of nodes when de-allocated
int mergeNode(void* address, int kval, int type);


#endif
