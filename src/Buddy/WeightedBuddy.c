#include "WeightedBuddy.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// declare the Available Space list array
ASL list[SIZES];

// declare the info array which will store the KVAL, TYPE and TAG informtation of a node
char info[NUM_NODES];

// base pointer which will hold the base address of the SP heap memory space
void* bptr = NULL;

// instrumentation
long num_allocation_requests = 0;
long num_deallocation_requests = 0;
long num_splits = 0;
long num_merges = 0;
long sum_allocated = 0;
long sum_requested = 0;

// getter functions for the various analysis data
double getInternalFragmentation(){
  return (sum_allocated - sum_requested)/(double)sum_requested;
}
long getNumSplits(){
  return num_splits;
}
long getNumMerges(){
  return num_merges;
}
long getNumAllocRequests(){
  return num_allocation_requests;
}
long getNumDeallocRequests(){
  return num_deallocation_requests;
}


// function to initialize the SP heap memory
int init_SPHeap(){

  // someone tried to initialize SP heap twice
  if(bptr){
    printf("\nError in init_SPHeap(): SP Heap already initialized.\n");
    return 0;
  }

  // create the lists for all possible sizes
  for(int i=0; i<SIZES; i++){
    list[i] = (ASL) malloc(sizeof(struct asl));
  }

  // initialize the info array
  for(int i=0; i<NUM_NODES; i++){
    info[i] = 0;
  }


  // allocate the SP Heap memory and store the address in base pointer
  bptr = malloc(NUM_NODES*sizeof(char));
  if(!bptr){
    printf("\nError in init_SPHeap(): Unable to allocate SP heap memory.\n");
    return 0;
  }

  // now let's create the first node of full size and add it to the ASL
  ASLNode node = NULL;
  node = createAslNode();
  if(!node){
    printf("\nError in init_SPHeap(): Unable to create a new asl node.\n");
    return 0;
  }

  // make sure the node points to the bptr
  node->ptr = bptr;

  // get the index for this node
  int index = getIndexFromAddress(node->ptr, bptr);

  // set the info byte for the newly created node
  setInfo(index, M, 0, 1);

  // add the node to the ASL
  insertIntoAsl(list[2*M -1], node);

  return 1;
}

// function to allocate memory as per the supplied size
void* SP_malloc(int size){

  // validate input
  if(size <=0 ){
    //printf("\nError in SP_malloc(): Invalid size.\n");
    return NULL;
  }

  // get the index for the ASL list based on the size
  int list_index = getIndexForListFromSize(size);
  if(list_index < 0){
    //printf("\nMessage from SP_malloc(): Not a perfect fit, taking next biggest possible.\n");
    list_index *= -1;
  }

  // get a free node from the list based on the index
  ASLNode node = getNodeFromList(list_index, bptr);
  if(!node){
    printf("\nError in SP_malloc(): Unable to satisfy the allocation request.\n");
    double AllocAreaPercentage = 100*(sum_allocated/(double)NUM_NODES);
    printf("\nError in SP_malloc(): External Fragmentation (Asize, AllocAreaPercentage): (%d, %lf).\n", size, AllocAreaPercentage);
    return NULL;
  }

  // get info array index based on the address
  int index = getIndexFromAddress(node->ptr, bptr);

  // compute the kval of the block
  int kval;
  if(list_index%2 != 0){
    kval = list_index/2 + 1;
  }
  else{
    kval = list_index/2 - 1;
  }

  // set the info accordingly
  setTag(index, 0);

  // compute the allocation size
  int alloc_size = 0;
  if(list_index%2 == 0){
    list_index = list_index/2;
    alloc_size = 3*pow(2, list_index-1);
  }else{
    list_index = (list_index+1)/2;
    alloc_size = pow(2, list_index);
  }

  // modify analysis data
  num_allocation_requests++;
  sum_allocated += alloc_size;
  sum_requested += size;

  return node->ptr;
}

// function to deallocate memory for the supplied address
void SP_free(void* address){

  // validate input
  if(!address){
    printf("\nError in SP_free(): Invalid input parameters.\n");
    return;
  }

  // modify analysis data
  num_deallocation_requests++;

  // get the index, kval and type of the ndoe to be de-allocated
  int index = getIndexFromAddress(address, bptr);
  int kval = (int)(info[index] & KVAL_CHECK);
  kval >>= 3;
  kval = kval&31;
  int type = (int)(info[index] & TYPE_CHECK);
  type >>= 1;

  // merge the node if possible and then add it to the ASL
  mergeNode(address, kval, type);

}

// function to perform the merging of nodes when de-allocated
int mergeNode(void* address, int kval, int type){

  // validate input
  if(!address){
    printf("\nError in mergeNode(): Invalid input parameters.\n");
    return 0;
  }

  // get the info index for the node to be de-allocated
  int index = getIndexFromAddress(address, bptr);

  // get the buddy index and check if it is free
  //int buddy_index = getBuddyIndex(address, index);
  // calculate the offset
  int offset = (char*)address - (char*)bptr;

  void* buddy_address = NULL;
  int merged_kval;

  // calculate the buddy address
  if(type == 3){
    if(offset%((int)pow(2, kval+2)) == 0){
      int x = (int)3*pow(2, kval);
      int y = offset + x;
      buddy_address = ((char*)bptr + y);
    }
    else{
      int x = (int)3*pow(2, kval);
      int y = offset - x;
      //int y=x;
      buddy_address = ((char*)bptr - y);
    }
  }
  else if(type == 2){
    int x = (int)pow(2, kval+1);
    int y = offset - x;
    buddy_address = ((char*)bptr - y);
  }
  else if(type == 1){
    int x = (int)pow(2, kval);
    int y = offset + x;
    buddy_address = ((char*)bptr + y);
  }


  // get the buddy info index, kval and type
  int buddy_index = getIndexFromAddress(buddy_address, bptr);
  int buddy_free = 0;
  if(buddy_index < 0){
    //printf("\nError in mergeNode(): Unable to calculate a correct buddy_index.\n");
    buddy_free = 0;
  }
  else{
    buddy_free = info[buddy_index] & TAG_CHECK;
  }

  // if the buddy is not free, we just add the node which is to be de-allocated back to ASL
  if(!buddy_free){

    //create a new ASL node which will be added to the list
    ASLNode node = createAslNode();
    node->ptr = address;

    // set the info accordingly
    setInfo(index, kval, type, 1);

    // calulate the offset and use it to get the list index
    int offset = (char*)address - (char*)bptr;
    int list_index;
    if(!offset){
      list_index = kval*2 + 2;
    }
    else{
      list_index = kval*2 - 1;
    }

    // insert the node into ASL
    insertIntoAsl(list[list_index], node);

    return 1;
  }
  // the buddy is free, so we must perform merging
  else{

    //modify analysis data
    num_merges++;

    // calculate the offset
    int offset = (char*)address - (char*)bptr;

    int buddy_kval = (int)(info[buddy_index] & KVAL_CHECK);
    buddy_kval >>= 3;
    buddy_kval = buddy_kval&31;
    int buddy_type = (int)(info[buddy_index] & TYPE_CHECK);
    buddy_type >>= 1;

    // calculate the buddy offset and use it to get its list index
    int buddy_offset = (char*)buddy_address - (char*)bptr;

    int buddy_list_index=0;

    if(buddy_kval == kval){
      buddy_list_index = (offset < buddy_offset) ? (buddy_kval*2 - 1) : ((buddy_kval+1)*2);
    }
    else{
      buddy_list_index = (buddy_kval*2 - 1);
    }

    // remove the buddy node from ASL as we are going to merge it
    ASLNode buddy_node = removeNodeFromAsl(list[buddy_list_index], buddy_address);
    if(!buddy_node){
      //printf("\nError in mergeNode(): Buddy free but not in list.\n");
      return 0;
    }

    // this means both are split from a 2^k block
    if(buddy_kval == kval){

      // the merged node will have the address and kval of the child which is its left child
      void* parent_address = (offset < buddy_offset) ? address : buddy_address;
      int parent_offset = (offset < buddy_offset) ? offset : buddy_offset;
      int parent_kval = (offset < buddy_offset) ? kval : buddy_kval;

      // get the parent info index
      int parent_index = getIndexFromAddress(parent_address, bptr);

      // compute the kval of the parent
      parent_kval+=2;

      // todo: not sure if the type being set is correct
      int parent_type = 0;

      int temp_index = parent_index;
      temp_index >>= parent_kval;
      int mask = 3;
      int two_bits = temp_index&mask;

      // assign parent type acc to last two bits
      if(two_bits == 3){
        parent_type = 3;
      }
      else if(two_bits == 2){
        parent_type = 2;
      }
      else{
        parent_type = 1;
      }

      // now we have found the address of the parent, so we can just call merge recursively on it
      return mergeNode(parent_address, parent_kval, parent_type);
    }
    // means the buddies are split from a 3*2^k node
    else{

      // parent will take this same address
      if(type == 1){

        // get all the required details of the parent
        void* parent_address = address;
        int parent_index = getIndexFromAddress(parent_address, bptr);
        int parent_kval = kval-1;
        int parent_type = 3;

        // now we have found the address of the parent, so we can just call merge recursively on it
        return mergeNode(parent_address, parent_kval, parent_type);
      }
      // parent will take the address of the buddy
      else if(type == 2){

        // get all the required details of the parent
        void* parent_address = buddy_address;
        int parent_index = getIndexFromAddress(parent_address, bptr);
        int parent_kval = buddy_kval;
        int parent_type = 3;

        // now we have found the address of the parent, so we can just call merge recursively on it
        return mergeNode(parent_address, parent_kval, parent_type);
      }

    }

  }


}


// function to create a new node
void* createAslNode(){

  // allocate memory for a new node
  ASLNode node = (ASLNode) malloc(sizeof(struct aslNode));
  if(!node){
    printf("\nError in createAslNode(): Unable to allocate memory for a new node.\n");
    return 0;
  }

  // initialize the pointers of the node
  node->ptr = NULL;
  node->next = NULL;
  node->prev = NULL;

  return node;
}

// function to get index from address for a node
int getIndexFromAddress(void* ptr, void* bptr){

  int index = (char*)ptr - (char*)bptr;

  return index;

}

// function to set the info byte of a node of given index
int setInfo(int index, int kval, int type, int tag){

  // validate inputs
  if(index < 0 || (type < 0 || type > 3) || (tag != 1 && tag != 0)){
    printf("\nError in setInfo(): Invalid input parameters.\n");
    return 0;
  }

  // set the last 5 bits of info to the value of kval
  char info_val = 0;
  info_val |= (kval << 3);
  info[index] = info_val;

  // set the first bit which represents tag
  if(tag == 1){
    info[index] |= 1UL << 0;
  }
  else if(tag == 0){
    info[index] &= ~(1UL << 0);
  }

  // set the 2nd and 3rd bits representing type
  if(type == 0){
    info[index] &= ~(1UL << 1);
    info[index] &= ~(1UL << 2);
  }
  else if(type == 1){
    info[index] |= 1UL << 1;
    info[index] &= ~(1UL << 2);
  }
  else if(type == 2){
    info[index] &= ~(1UL << 1);
    info[index] |= 1UL << 2;
  }
  else if(type == 3){
    info[index] |= 1UL << 1;
    info[index] |= 1UL << 2;
  }

  return 1;

}

// function to set the info byte of a node of given index
int setTag(int index, int tag){

  // validate inputs
  if(index < 0 || (tag != 1 && tag != 0)){
    printf("\nError in setTag(): Invalid input parameters.\n");
    return 0;
  }

  // set the last 5 bits of info to the value of kval
  char info_val = info[index];

  // set the first bit which represents tag
  if(tag == 1){
    info[index] |= 1UL << 0;
  }
  else if(tag == 0){
    info[index] &= ~(1UL << 0);
  }

  return 1;
}


// function to get teh ASL index depending on the size of the node
int getIndexForListFromSize(int size){

  int k = log(size)/log(2);

  //size fits with 3*2^(k-1)
  if(size == 3*pow(2,k-1)){
    return 2*k;
  }
  //size fits with 2^k
  else if(size == pow(2,k)){
    return 2*k - 1;
  }
  // size is somewhere between 2^k and 3*2^(k-1)
  // so it would require the bigger size of two
  // negative denotes it is not exact
  else if(size < 3*pow(2,k-1)){
    //printf("\nMessage from getIndexForListFromSize(): Invalid size.\n");
    //printf("\nMessage from getIndexForListFromSize(): Returning index of next biggest size (in negative).\n");
    return -2*k;
  }
  // size require 2^(k+1)
  // negative denotes it is not exact
  else{
    //printf("\nMessage from getIndexForListFromSize(): Invalid size.\n");
    //printf("\nMessage from getIndexForListFromSize(): Returning index of next biggest size (in negative).\n");
    return -2*(k+1) - 1;
  }

}

// add the node to the ASL
int insertIntoAsl(ASL list, ASLNode node){

  //validate inputs
  if(!list){
    //printf("\nError in insertIntoAsl(): List parameter passed as null.\n");
    return 0;
  }
  if(!node){
    //printf("\nError in insertIntoAsl(): Node parameter passed as null.\n");
    return 0;
  }

  // if list is empty
  if(!list->head){
    list->head = node;
    list->tail = node;
    return 1;
  }

  // add the node at the end
  list->tail->next = node;
  node->prev = list->tail;
  list->tail = node;

  return 1;

}

// function to search for the address and remove it from the list
ASLNode removeNodeFromAsl(ASL list, void* address){

  // validate inputs
  if(!list){
    //printf("\nError in removeNodeFromAsl(): List parameter passed as null.\n");
    return NULL;
  }
  if(!address){
    //printf("\nError in removeNodeFromAsl(): Address parameter passed as null.\n");
    return NULL;
  }

  // if the list is empty, its an erro
  if(!list->head){
    printf("\nError in removeNodeFromAsl(): No nodes in list.\n");
    return NULL;
  }

  // get the pointer to iterate over the list
  ASLNode curr = list->head;

  // iterate and find the node to be removed
  while(curr){
    if(curr->ptr == address){
      break;
    }else{
      curr = curr->next;
    }
  }

  // error if node not in the list
  if(!curr){
    //printf("\nError in removeNodeFromAsl(): Address not in list.\n");
    return NULL;
  }

  //remove the node
  ASLNode prev = curr->prev;
  if(!prev){
    list->head = curr->next;
    if(curr->next){
      curr->next->prev = prev;
    }
  }
  else{
    prev->next = curr->next;
    if(curr->next){
      curr->next->prev = prev;
    }
  }


  //return the node
  curr->prev = curr->next = NULL;
  return curr;
}

// function to compute the index of buddy for the info array based on the address and index of the node
int getBuddyIndex(void* address, int index){

  //validate input
  if(!address){
    printf("\nError in getBuddyIndex(): Invalid input parameters.\n");
    return -1;
  }

  // get the details of the node
  int kval = (int)(info[index] & KVAL_CHECK);
  kval >>= 3;
  kval = kval&31;
  int type = (int)(info[index] & TYPE_CHECK);
  type >>= 1;
  int offset = (char*)address - (char*)bptr;

  void* buddy_address = NULL;

  // todo: not sure about type 0

  // get the buddy address based on types
  if(type == 3 || type == 0){
    if(!offset){
      offset |= 3UL << kval;
      buddy_address = ((char*)bptr + offset);
    }
    else{
      offset &= ~(1UL << (kval));
      offset &= ~(1UL << (kval+1));
      buddy_address = ((char*)bptr + offset);
    }
  }
  else if(type == 2){
    offset &= ~(1UL << (kval+1));
    buddy_address = ((char*)bptr + offset);
  }
  else if(type == 1){
    offset |= 1UL << kval;
    buddy_address = ((char*)bptr + offset);
  }

  // get buddy index based on the address
  int buddy_index = getIndexFromAddress(buddy_address, bptr);

  return buddy_index;
}

// function to get a node from ASL for a given index
ASLNode getNodeFromList(int list_index, void* bptr){

  ASLNode node = NULL;

  //we have a free node of the required size available
  if(list[list_index] && list[list_index]->head){
    node = list[list_index]->head;
    list[list_index]->head = list[list_index]->head->next;
    if(list[list_index]->head){
      list[list_index]->head->prev = NULL;
    }
    return node;
  }

  // find the next available node that can be split
  int split_index = list_index;
  while(1){

    if(split_index < 2*M && !list[split_index]){
      split_index++;
      if(split_index == 1){
        split_index++;
      }
    }
    else if(split_index < 2*M && !list[split_index]->head){
      split_index++;
      if(split_index == 1){
        split_index++;
      }
    }
    else{
      break;
    }
  }

  if(split_index == 2*M){
    printf("\nError in getNodeFromList(): No nodes to split.\n");
    return NULL;
  }

  // iteratively split nodes till we reach required size
  while(split_index != list_index){

    int left_index, right_index;

    if(!splitNode(split_index, &left_index, &right_index, bptr)){
      printf("\nError in getNodeFromList(): Unable to split node.\n");
      return NULL;
    }

    if(right_index >= list_index){
      split_index = right_index;
    }
    else{
      split_index = left_index;
    }

  }

  // now we have a free node of the required size available after splitting
  if(list[list_index] && list[list_index]->head){
    node = list[list_index]->head;
    list[list_index]->head = list[list_index]->head->next;
    if(list[list_index]->head){
      list[list_index]->head->prev = NULL;
    }
    return node;
  }
  else{
    printf("\nError in getNodeFromList(): Unable to split node.\n");
    return NULL;
  }

}

// function to split a node to two
int splitNode(int split_index, int* left_index, int* right_index, void* bptr){

  //validate inputs
  if(!list || !list[split_index]->head){
    printf("\nError in splitNode(): Invalid input parameters.\n");
    return 0;
  }

  //modify analysis data
  num_splits++;

  // get the node which is to be split
  ASLNode split_node = list[split_index]->head;
  list[split_index]->head = split_node->next;
  if(list[split_index]->head){
    list[split_index]->head->prev = NULL;
  }
  split_node->next = NULL;

  //these nodes will be the result of splitting
  ASLNode left_node = createAslNode();
  ASLNode right_node = createAslNode();

  // even means it is a node of the form 2^k
  if(split_index%2 != 0){

    // compute the left and right indices
    int k = split_index/2 + 1;
    if(k==2){
      *right_index = 0;
    }
    else{
      *right_index = split_index-4;
    }
    *left_index = split_index-1;

    // compute the addresses for the lefta nd right nodes
    int right_node_k = k-2;
    int offset = (char*)split_node->ptr - (char*)bptr;
    offset |= 3UL << right_node_k;
    right_node->ptr = ((char*)bptr + offset);
    left_node->ptr = split_node->ptr;

    // isnert the nodes into the lists
    insertIntoAsl(list[*left_index], left_node);
    insertIntoAsl(list[*right_index], right_node);

    //free(split_node);
    split_node = NULL;

    // set the info accordingly
    int left_child_index = getIndexFromAddress(left_node->ptr, bptr);
    int right_child_index = getIndexFromAddress(right_node->ptr, bptr);
    setInfo(left_child_index, k-2, 3, 1);
    setInfo(right_child_index, k-2, 3, 1);

    return 1;
  }
  // odd means it is a node of the form 3*2^k
  else{

    // compute the left and right indices
    int k = split_index/2 -1;
    if(k==0){
      *right_index = 0;
    }
    else{
      *right_index = split_index-3;
    }
    *left_index = split_index-1;

    // compute the addresses for the lefta nd right nodes
    int right_node_k = k;
    int offset = (char*)split_node->ptr - (char*)bptr;
    offset |= 2UL << right_node_k;
    right_node->ptr = ((char*)bptr + offset);
    left_node->ptr = split_node->ptr;

    // isnert the nodes into the lists
    insertIntoAsl(list[*left_index], left_node);
    insertIntoAsl(list[*right_index], right_node);

    //free(split_node);
    split_node = NULL;

    // set the info accordingly
    int left_child_index = getIndexFromAddress(left_node->ptr, bptr);
    int right_child_index = getIndexFromAddress(right_node->ptr, bptr);
    setInfo(left_child_index, k+1, 1, 1);
    setInfo(right_child_index, k, 2, 1);

    return 1;
  }

}
