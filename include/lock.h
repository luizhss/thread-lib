#ifndef LOCK_H
#define LOCK_H

#include <queue.h>

typedef struct {
	enum {
	      UNLOCKED,
	      LOCKED,
	} status;
	node_t block_queue;
} lock_t;

void lock_init(lock_t *);
void lock_acquire(lock_t *);
void lock_release(lock_t *);
void block();
void unblock(lock_t *);

#endif
