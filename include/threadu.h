#ifndef THREADU_H
#define THREADU_H

typedef enum {
    FALSE, TRUE
} bool_t;

typedef struct thr {
	void 	*tcb;
} thread_t;

int thread_create(thread_t *, void *(*start_routine)(void *), void *);
int thread_yield();
int thread_join(thread_t *, int *); 
int thread_init();

void thread_exit(int);
void set_timer();

#endif
