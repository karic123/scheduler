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
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
        q->compare = comparer;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
        struct node* new = malloc(sizeof(struct node));
        new->data = ptr;
        new->prev = NULL;
        new->next = NULL;
        int i = -1;

        if(isEmpty(q)){
                q->head = new;
                q->tail = new;
                i++;
        }
        else{
                i++;
                int priority = 0;
                struct node* temp = q->head;
                while(temp != NULL){
                        priority = q->compare(temp->data, new->data);
                        if(priority > 0){
                                if(temp != q->head){
                                        new->prev = temp->prev;
                                        new->prev->next = new;
                                }
                                else{
                                        q->head = new;
                                }
                                new->next = temp;
                                temp->prev = new;
                                temp = NULL;
                        }
                        else if(temp == q->tail){
                                new->prev = temp;
                                temp->next = new;
                                q->tail = new;
                                temp = NULL;
                                i++;
                        }
                        else{
                                i++;
                                temp = temp->next;
                        }
                }
        }
        q->size++;
        return i;
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
        if( isEmpty(q)){
                return NULL;
        }
        else{
                return q->head->data;
        }
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
        if( isEmpty(q)){
                return NULL;
        }
        else{
                return priqueue_remove_at(q,0);
        }

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
        if(index >= q->size || index < 0){
                return NULL;
        }
        else{
                struct node* temp = traverseTo(q,index);
                return temp->data;
        }
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
        if(isEmpty(q) || ptr==NULL){
                return 0;
        }
        else{
                int index = 0;
                int count = 0;
                struct node* temp = q->head;
                while(temp != NULL){
                        if(ptr == temp->data){
                                priqueue_remove_at(q,index);
                                count++;
                        }
                        index++;
                }
                return count;
        }

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
        if(index >= q->size || index < 0){
                return NULL;
        }
        else{
                struct node* temp = traverseTo(q,index);
                if(q->size == 1){
                        q->head = NULL;
                        q->tail = NULL;
                }
                else{
                        if(temp == q->head){
                                temp->next->prev = NULL;
                                q->head = temp->next;
                        }
                        else if(temp == q->tail){
                                temp->prev->next = NULL;
                                q->tail = temp->prev;
                        }
                        else{
                                temp->prev->next = temp->next;
                                temp->next->prev = temp->prev;
                        }
                }
                temp->next = NULL;
                temp->prev = NULL;
                void* data = temp->data;
                free(temp);
                q->size--;
                return data;
        }
}


/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
        return q->size;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
        while(q->size > 0){
                priqueue_poll(q);
        }
}

bool isEmpty(priqueue_t *q)
{
        return (q->size == 0);
}

void * traverseTo(priqueue_t *q, int index){
        struct node* trav = q->head;
        for(int i=0; i<index; i++){
                trav = trav->next;
        }
        return trav;
}
