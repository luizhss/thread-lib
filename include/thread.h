#ifndef THREAD_H
#define THREAD_H

#include <stdint.h>
#include "threadu.h"

#define NUMBER_OF_REGISTERS	15
#define STACK_SIZE		4096

void scheduler_entry();
void exit_handler();

typedef enum {
	      FIRST_TIME,
	      READY,
	      BLOCKED,
	      EXITED
} status_t;

typedef struct tcb {
	uint64_t regs[NUMBER_OF_REGISTERS]; //120 bytes - 8 de 15
	uint64_t rflags; // 8 bytes
	uint64_t *stack_pointer; // 8 bytes
	uint64_t *stack; // 8 bytes
	int result;
	int id;
	uint64_t time_spend;
	status_t status;
} tcb_t;

#endif
