/*
exceptions.h: various exception handlers set up in the IDT
*/

#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H

#include "x86_desc.h"
#include "syscall.h"

/* order exceptions are arranged in IDT */

extern void exc_division_error(void);
extern void exc_debug(void);
extern void exc_non_maskable_interrupt(void);
extern void exc_breakpoint(void);
extern void exc_overflow(void);
extern void exc_range_bound_exceeded(void);
extern void exc_invalid_opcode(void);
extern void exc_device_not_available(void);
extern void exc_double_fault(void);
extern void coprocessor_segment_overrun(void);
extern void invalid_tss(void);
extern void segment_not_present(void);
extern void stack_segment_fault(void);
extern void general_protection_fault(void);
extern void page_fault(void);
extern void floating_point_exc(void);
extern void alignment_check(void);
extern void machine_check(void);
extern void simd_floating_point_exc(void);


extern void div_asm(void);
extern void debug_asm(void);
extern void nmi_asm(void);
extern void breakpoint_asm(void);
extern void overflow_asm(void);
extern void range_bound_asm(void);
extern void invalid_opcode_asm(void);
extern void device_not_avail_asm(void);
extern void double_fault_asm(void);
extern void coprocessor_segment_overrun_asm(void);
extern void invalid_tss_asm(void);
extern void segment_not_present_asm(void);
extern void stack_segment_fault_asm(void);
extern void general_protection_fault_asm(void);
extern void page_fault_asm(void);
extern void floating_point_exc_asm(void);
extern void alignment_check_asm(void);
extern void machine_check_asm(void);
extern void simd_asm(void);

void halt_and_return(void);

volatile uint32_t exceptions_flag;

extern void pit_asm(void);
extern void kbd_asm(void);
extern void rtc_asm(void);

#endif
