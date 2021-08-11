#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef struct node {
   void *tcb;
   struct node* next;
} node_t;

// Initialize a queue
void queue_init(node_t *);

// Remove and return the item at the front of the queue Return NULL if the
// queue is empty
node_t *dequeue(node_t *);

// Add item to the back of the queue
void enqueue(node_t *, node_t *);

// Determine if the queue is empty.
// Returns 1 if the queue is empty.
// Returns 0 otherwise.
int is_empty(node_t *);

// Returns the first item in the queue
// Returns NULL if the queue is empty
node_t *peek(node_t *);

// A comparison function should return:
//  1, if a is less-than-or-equal-to b
//  0, otherwise.
typedef int (*node_lte)(node_t *, node_t *);

int comp_nodes(node_t*, node_t*);

// Insert a item in the queue using the given comp function.
// It must keep the list sorted.
// This is a simple linear-time algorithm.
void enqueue_sort(node_t *, node_t *, node_lte);

#endif
