#include "exceptions.h"
#include "lib.h"

/*
*This file contains the first 19 exception handlers listed in the IDT.
*Each handler prints the name of the exception and then halts
*effectively 'blue screening' the kernel
*/
void halt_and_return(void){
	exceptions_flag = 1;
	halt(0);
}

void exc_division_error(){
	printf("Divide by Zero Exception\n");
	halt_and_return();
	}

void exc_debug(){
	printf("exc debug\n");
	halt_and_return();
	}

void exc_non_maskable_interrupt(){
	printf("exception non-maskable interrupt\n");
	halt_and_return();

	}
void exc_breakpoint(){
	printf("exception breakpoint\n");
	halt_and_return();
	}

void exc_overflow(){
	printf("exception overflow\n");
	halt_and_return();
	}

void exc_range_bound_exceeded(){
	printf("exception range bound exceeded\n");
	halt_and_return();
	}

void exc_invalid_opcode(){
	printf("exception invalid opcode\n");
	halt_and_return();
	}

void exc_device_not_available(){
	printf("exception device not available\n");
	halt_and_return();
	}

void exc_double_fault(){
	printf("exception double fault\n");
	halt_and_return();
	}

void coprocessor_segment_overrun(void){
	printf("coprocessor segment overrun\n");
	halt_and_return();
	}

void invalid_tss(void){
	printf("invalid TSS\n");
	halt_and_return();
	}
	
void segment_not_present(void){
	printf("segment not present\n");
	halt_and_return();
	}

void stack_segment_fault(void){
	printf("Segmentation Fault\n");
	halt_and_return();
	}

void general_protection_fault(void){
	printf("general protecton fault\n");
	halt_and_return();
	}

void page_fault(void){
	printf("page fault\n");
	halt_and_return();
	}

void floating_point_exc(void){
	printf("x87 floating point exception\n");
	halt_and_return();
	}

void alignment_check(void){
	printf("alignment check\n");
	halt_and_return();
	}

void machine_check(void){
	printf("machine check\n");
	halt_and_return();
	}

void simd_floating_point_exc(void){
	printf("simd floating point exception\n");
	halt_and_return();
	}
