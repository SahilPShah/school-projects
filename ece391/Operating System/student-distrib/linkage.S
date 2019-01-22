/*
*Assembly linkage for each exception
*maintains stack, calls C handler, and returns from interrupt using iret
*/


.globl div_asm
.globl debug_asm
.globl nmi_asm
.globl breakpoint_asm
.globl overflow_asm
.globl range_bound_asm
.globl invalid_opcode_asm
.globl device_not_avail_asm
.globl double_fault_asm
.globl coprocessor_segment_overrun_asm
.globl invalid_tss_asm
.globl segment_not_present_asm
.globl stack_segment_fault_asm
.globl general_protection_fault_asm
.globl page_fault_asm
.globl floating_point_exc_asm
.globl alignment_check_asm
.globl machine_check_asm
.globl simd_asm

.globl pit_asm
.globl kbd_asm
.globl rtc_asm

div_asm:
  pushal
  call exc_division_error
  popal
  iret

debug_asm:
  pushal
  call exc_debug
  popal
  iret

nmi_asm:
  pushal
  call exc_non_maskable_interrupt
  popal
  iret

breakpoint_asm:
  pushal
  call exc_breakpoint
  popal
  iret

overflow_asm:
  pushal
  call exc_overflow
  popal
  iret

range_bound_asm:
  pushal
  call exc_range_bound_exceeded
  popal
  iret

invalid_opcode_asm:
  pushal
  call exc_invalid_opcode
  popal
  iret

device_not_avail_asm:
  pushal
  call exc_device_not_available
  popal
  iret

double_fault_asm:
  pushal
  call exc_double_fault
  popal
  iret

coprocessor_segment_overrun_asm:
  pushal
  call coprocessor_segment_overrun
  popal
  iret

invalid_tss_asm:
  pushal
  call invalid_tss
  popal
  iret

segment_not_present_asm:
  pushal
  call segment_not_present
  popal
  iret

stack_segment_fault_asm:
  pushal
  call stack_segment_fault
  popal
  iret

general_protection_fault_asm:
  pushal
  call general_protection_fault
  popal
  iret

page_fault_asm:
  pushal
  call page_fault
  popal
  iret

floating_point_exc_asm:
  pushal
  call floating_point_exc
  popal
  iret

alignment_check_asm:
  pushal
  call alignment_check
  popal
  iret

machine_check_asm:
  pushal
  call machine_check
  popal
  iret

simd_asm:
  pushal
  call simd_floating_point_exc
  popal
  iret

pit_asm:
  pushal          # push current process GPR
  movl %esp, %eax
  movl %ebp, %ebx
  call pit_handler_wrapper  # call the handler to invoke the scheduler
  movl %eax, %esp
  movl %ebx, %ebp
  popal           # pop next process's GPR's
  iret            # return to next process (instead of the current process)

kbd_asm:
  pushal
  call kbd_handler
  popal
  iret

rtc_asm:
  pushal
  call rtc_handler
  popal
  iret
