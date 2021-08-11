#include <queue.h>
#include <stdio.h>

/*
  This   file    contains   a    function   definitions    for   queue
  manipulation. 
 */

// Inicializes a queue
void queue_init(node_t * queue){  
  queue->tcb = NULL;
  queue->next = NULL;
}

// Returns the first element of the queue
node_t *dequeue(node_t * queue){
  if(is_empty(queue))
    return NULL;

  node_t* ptr = queue->next;
  queue->next = queue->next->next;

  ptr->next = NULL;
  return ptr;
}

// Inserts a node in a queue
void enqueue(node_t * queue, node_t * item){
  node_t * ptr;
  ptr = queue;

  while(ptr->next != NULL){
    ptr = ptr->next;
  }

  ptr->next = item;
  item->next = NULL;
}

// Checks if a queue is empty
int is_empty(node_t *queue){
  if(queue->next == NULL) return 1;
	return 0;
}

// Returns the first element of the queue
node_t *peek(node_t *queue){
  if(is_empty(queue))
    return NULL;
  return queue->next;
}

// Enqueue a item into the queue, but keeping the list ordered
void enqueue_sort(node_t *q, node_t *item, node_lte comp){
  node_t *ptr = q;

  while(ptr->next != NULL && comp(ptr->next, item)){
    ptr = ptr->next;
  }

  item->next = ptr->next;
  ptr->next = item;
}
