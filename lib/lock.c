#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <lock.h>
#include <queue.h>
#include <threadu.h>
#include <thread.h>
#include <util.h>

extern node_t ready_queue;
extern tcb_t * current_running;
extern bool FAIRNESS;
extern uint64_t time_begin;

enum {
      SPIN = FALSE,
};

// Inicializes a lock
void lock_init(lock_t * l) {
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		l->status = UNLOCKED;
		queue_init(&(l->block_queue));
	}
}

// Acquires a lock if it is available, otherwise, blocks the thread
void lock_acquire(lock_t * l){
	if (SPIN) {
		while (LOCKED == l->status)
			thread_yield();
		l->status = LOCKED;
	} else {

		if (l->status == LOCKED){
			current_running->status = BLOCKED;
			node_t *new_node = (node_t*) malloc(sizeof(node_t));
			new_node->tcb = current_running;
			new_node->next = NULL;

			enqueue(&l->block_queue, new_node);
			thread_yield();
		}else{
			l->status = LOCKED;
		}
	}
}

// Releases a lock and unlocks one thread from the lock's
// blocking list
void lock_release(lock_t * l)
{
	if (SPIN) {
		l->status = UNLOCKED;
	} else {
		if(is_empty(&(l->block_queue))){
			l->status = UNLOCKED;
		}else{
			((tcb_t *) l->block_queue.next->tcb)->status = READY;
			node_t * node_th = dequeue(&l->block_queue);
			node_th->next = NULL;

			if(FAIRNESS){
				enqueue_sort(&ready_queue, node_th, comp_nodes);
			}else{
				enqueue(&ready_queue, node_th);
			}
			//l->status keeps LOCKED
		}
	}
}
