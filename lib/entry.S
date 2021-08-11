	.section .data
scratch : .quad	0
	
	.text
	.globl	scheduler_entry
	.extern current_running

// This function executes the following steps:
// 1. saves the context of the running thread
// 2. calls the scheduler to select the thread to be executed
// 3. restores the context of the selected thread
scheduler_entry:
	
	// Save context

	movq %rsp, scratch
	movq current_running, %rsp
	leaq 136(%rsp), %rsp

	pushq scratch
	pushfq
	pushq %r15
	pushq %r14
	pushq %r13
	pushq %r12
	pushq %r11
	pushq %r10
	pushq %r9
	pushq %r8
	pushq %rbp
	pushq %rdi
	pushq %rsi
	pushq %rbx
	pushq %rdx
	pushq %rcx
	pushq %rax

	movq scratch, %rsp


	call scheduler

	// Restore context

	movq current_running, %rsp

	popq %rax
	popq %rcx
	popq %rdx
	popq %rbx
	popq %rsi
	popq %rdi
	popq %rbp
	popq %r8
	popq %r9
	popq %r10
	popq %r11
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	popfq
	popq %rsp

	ret
