#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "queue.h"
#include "thread.h"
#include "util.h"

node_t ready_queue;
tcb_t	*current_running;

int tid_global = 0;
uint64_t time_begin;

bool FAIRNESS = false;

// Compares two node_t types based on their time_spend
int comp_nodes(node_t* a, node_t* b){
  return (((tcb_t *) a->tcb)->time_spend <= ((tcb_t *) b->tcb)->time_spend);
}

 /*
  Initializes  the  thread library  and creates  a
  thread for  the main function. Returns  0 on success, or  -EINVAL if
  the library has already been initialized.
 */
int thread_init() {

	if(tid_global > 0){
		return -EINVAL;
	}

	queue_init(&ready_queue);

	current_running  = (tcb_t*) malloc(sizeof(tcb_t));

	for(int i = 0; i < NUMBER_OF_REGISTERS; i++)
		current_running->regs[i] = 0;
	current_running->rflags = 0;
	current_running->stack_pointer = 0;
	current_running->stack = NULL;
	current_running->result = -1;
	current_running->id = tid_global++;
	current_running->status = READY;

	if(FAIRNESS){
		current_running->time_spend = 0;
		time_begin = get_timer();
	}

	return 0;
}

// Creates a new thread and inserts in the ready queue.
int thread_create(thread_t *thread, void *(*start_routine)(void *), void *arg) {
	
	thread->tcb = (void *) malloc(sizeof(tcb_t));

	for(int i = 0; i < 15; i++){
		((tcb_t *) thread->tcb)->regs[i] = 0;
	}
	((tcb_t *) thread->tcb)->regs[5] = (uint64_t) arg; // %rdi
	((tcb_t *) thread->tcb)->rflags = 0; 

	((tcb_t *) thread->tcb)->stack = (uint64_t *) malloc(STACK_SIZE);
	((tcb_t *) thread->tcb)->stack_pointer = ((tcb_t *) thread->tcb)->stack + STACK_SIZE - 4;
	*(--((tcb_t *) thread->tcb)->stack_pointer) = (uint64_t) &exit_handler;
	*(--((tcb_t *) thread->tcb)->stack_pointer) = (uint64_t) start_routine;
	
	((tcb_t *) thread->tcb)->result = -1;
	((tcb_t *) thread->tcb)->id = tid_global++;
	((tcb_t *) thread->tcb)->status = READY; 


	node_t* new_node = (node_t*) malloc(sizeof(node_t));
	new_node->tcb = thread->tcb;
	new_node->next = NULL;


	if(FAIRNESS){
		((tcb_t *) thread->tcb)->time_spend = 0;
		*(--((tcb_t *) thread->tcb)->stack_pointer) = (uint64_t) &set_timer;
		enqueue_sort(&ready_queue, new_node, comp_nodes);
	}else{
		enqueue(&ready_queue, new_node);
	}

	return 0;
}

// Yields the CPU to another thread if the ready queue is
// not empty
int thread_yield(){

	uint64_t time_now = get_timer();
	if(FAIRNESS){
		current_running->time_spend += (time_now-time_begin);
	}

	if(is_empty(&ready_queue)){
		return 0;
	}

	if(current_running->status == READY){
		node_t* new_node = (node_t*) malloc(sizeof(node_t));

		new_node->tcb = (void *) current_running;
		new_node->next = NULL;

		if(FAIRNESS){
			enqueue_sort(&ready_queue, new_node, comp_nodes);
		}else{
			enqueue(&ready_queue, new_node);
		}
	}

	scheduler_entry();

	if(FAIRNESS){
		time_begin = get_timer();
	}

	return 0;
}

// Waits for a thread to finish and free its allocations
int thread_join(thread_t *thread, int *retval){
	while(((tcb_t *) thread->tcb)->status != EXITED){
		thread_yield();	
	}

	// uncomment the following code to see the time spend per thread
	/*if(FAIRNESS){
		gotoxy(1, ((tcb_t *) thread->tcb)->id);
		printf("THREAD %d spent %lu clock cycles\n", ((tcb_t *) thread->tcb)->id, ((tcb_t *) thread->tcb)->time_spend);
	}*/

	if(retval != NULL)
		*retval = ((tcb_t *) thread->tcb)->result;

	free(((tcb_t *) thread->tcb)->stack);
	free(thread->tcb);
	return 0;
}

// Marks a thread as EXITED and don't push it into the ready queue
void thread_exit(int status){

	current_running->status = EXITED;
	current_running->result = status;

	if(FAIRNESS){
		uint64_t time_now = get_timer();
		current_running->time_spend += (time_now-time_begin);
	}

	scheduler_entry();
}

// Selects the next thread to execute
void scheduler(){

	node_t *next_execution = dequeue(&ready_queue);
	current_running = (tcb_t *) next_execution->tcb;

	free(next_execution);
}

// This functions is called in the case the functions doesn't call thread_exit() 
void exit_handler(){
	thread_exit(0);
}

// Save the first time a function is called (for FAIRNESS)
void set_timer(){
	time_begin = get_timer();
}

