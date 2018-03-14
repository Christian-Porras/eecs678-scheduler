/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.
  
  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->m_root = 0;
  q->m_size = 0;
  q->m_comp = comparer;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
	Node * newNode = malloc(sizeof(Node));
  newNode->pointer = ptr;
  newNode->next = NULL;

  //empty queue
  if(q->m_size == 0){
    q->m_root = newNode;
    q->m_size = 1;

    return 0;
  }


  Node * temp = q->m_root;
  Node * parent = NULL;

  int index = 0;

  //traverse until correct index to insert is found
  while(temp != NULL && q->m_comp(temp->pointer, ptr) < 0){
    parent = temp;
    temp = temp->next;
    index ++;
  }

  if(index == 0){
    newNode->next = q->m_root;
    q->m_root = newNode;
    q->m_size++;
    return 0;
  }

  parent->next = newNode;
  newNode->next = temp;
  q->m_size++;

    return index;
}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
  if(q->m_root != NULL){
    return q->m_root->pointer;
  }
	return NULL;
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
  if(q->m_size == 0){
    return NULL;
  }

  Node * temp = q->m_root;
  void * ptr = 0;
  if(temp != NULL){
    q->m_root = temp->next;
  }
  else{
    q->m_root = NULL;
  }

  ptr = temp->pointer;
  free(temp);
  q->m_size--;
	return ptr;
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
  if(index >= q->m_size){
    return NULL;
  }
  else{
    Node * temp = q->m_root;

    for(int i = 1; i <= index; i++){
      temp = temp->next;
    }
    return temp->pointer;
  }
	return NULL;
}


/**
  Removes all instances of ptr from the queue. 
  
  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
  if(q->m_size == 0){
    return 0;
  }

  //m_root == ptr
  if(q->m_comp(ptr, q->m_root->pointer) == 0){
    Node * temp = q->m_root;
    q->m_root = q->m_root->next;
    q->m_size--;
    free(temp);
    //continue traversing through list looking for ptr
    return(priqueue_remove(q, ptr) + 1);
  }

  Node * cur = q->m_root->next;
  Node * parent = q->m_root;

  int numRemoved = 0;
  while(cur != NULL){
    if(q->m_comp(ptr, cur) == 0){
      Node * temp = cur;
      cur = cur->next;
      parent->next = cur;
      free(temp);
      numRemoved++;
      q->m_size--;
    }
    else{
      parent = cur;
      cur = cur->next;
    }

  }
	return numRemoved;
}


/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{
  if(q->m_size <= index){
    return NULL;
  }

  q->m_size--;
  Node * temp = q->m_root;

  if(index == 0){
    q->m_root = temp->next;
    return temp;
  }

  Node * parent = temp;
  temp = temp->next;

  for(int i = 1; i <= index; i++){
    parent = temp;
    temp = temp->next;
  }

  parent->next = temp->next;
	return temp;
}


/**
  Returns the number of elements in the queue.
 
  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
	return q->m_size;
}


/**
  Destroys and frees all the memory associated with q.
  
  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
  while(q->m_size > 0){
    void * temp = priqueue_remove_at(q, 0);
    free(temp);
  }
}
