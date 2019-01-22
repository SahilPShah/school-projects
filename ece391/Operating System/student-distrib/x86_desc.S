# x86_desc.S - Set up x86 segment descriptors, descriptor tables
# vim:ts=4 noexpandtab

#define ASM     1
#include "x86_desc.h"


.text

.globl ldt_size, tss_size
.globl gdt_desc, ldt_desc, tss_desc
.globl tss, tss_desc_ptr, ldt, ldt_desc_ptr
.globl gdt_ptr, gdt_desc_ptr
.globl idt_desc_ptr, idt
.globl sys_call_asm
.globl sys_call_table

.align 4

tss_size:
    .long tss_bottom - tss - 1

ldt_size:
    .long ldt_bottom - ldt - 1

    .word 0 # Padding
ldt_desc:
    .word KERNEL_LDT
    .long ldt

    .align 4
tss:
_tss:
    .rept 104
    .byte 0
    .endr
tss_bottom:

    .align  16
gdt:
_gdt:

    # First GDT entry cannot be used
    .quad 0

    # NULL entry
    .quad 0

    # Segmentation will not be used
    # CS and DS both are 0-4GB r/w segments
    #
    # The layout is (from Intel IA-32 reference manual):
    #  31        24 23  22  21  20  19   16 15  14 13 12  11   8 7          0
    # |----------------------------------------------------------------------|
    # |            |   | D |   | A |  Seg  |   |  D  |   |      |            |
    # | Base 31:24 | G | / | 0 | V | Limit | P |  P  | S | Type | Base 23:16 |
    # |            |   | B |   | L | 19:16 |   |  L  |   |      |            |
    # |----------------------------------------------------------------------|
    #
    # |----------------------------------------------------------------------|
    # |                                    |                                 |
    # | Base 15:0                          | Segment Limit 15:0              |
    # |                                    |                                 |
    # |----------------------------------------------------------------------|

gdt_ptr:
    # Set up an entry for kernel CS
    .quad 0x00CF9A000000FFFF

    # Set up an entry for kernel DS
    .quad 0x00CF92000000FFFF

    # Set up an entry for user CS
    .quad 0x00CFFA000000FFFF

    # Set up an entry for user DS
    .quad 0x00CFF2000000FFFF

    # Set up an entry for TSS
tss_desc_ptr:
    .quad 0

    # Set up one LDT
ldt_desc_ptr:
    .quad 0

gdt_bottom:



    .align 16
ldt:
    .rept 4
    .quad 0
    .endr
ldt_bottom:

.align 4
    .word 0 # Padding
idt_desc_ptr:
    .word idt_bottom - idt - 1
    .long idt

.align 4
    .word 0 # Padding
gdt_desc_ptr:
	.word gdt_bottom - gdt - 1
	.long gdt

    .align  16
idt:
_idt:
    .rept NUM_VEC
    .quad 0
    .endr

idt_bottom:



# write_asm (function, buf, n_bytes)
# function should not be zero

# the actual system call
# args: in %eax, the vector to the call
# in %ebx, %ecx, %edx: depends on the call
# return values: depends on function called
/*
The system call function begins by saving all registers to the stack. It next checks whether the program is under
the control of a debugger, in which case the debugger is allowed to intercept the system call before the system call
executes on behalf of the program. The EAX register is used to select the specific system call, and its value is checked
to ensure that a valid system call was requested. If valid, EAX is used as an index into a jump table, syscall table,
which also appears in entry.S, and an indirect call is made to the code for the specific system call. After this call
returns, the function does a little cleanup and returns control to the program. (course notes)

Eventually, all ten system calls must be supported via a common IDT entry, so you will have to set up some generic
assembly linkage along the lines of that used in Linux, including syscall value checking, register save and restore,
and a jump table to C (CHeckpoint 3 documentation)

pushes arguments on stack
*/

/* assembly linkage */
.align 16
sys_call_asm:
  pushl %edx      # holds args
  pushl %ecx      #
  pushl %ebx

  # check to see if %eax is in valid range (between 0 and NUM_SYSC(10))
  cmpl $0, %eax             # any sys call # < 1 is invalid
  jle sys_call_error
  cmpl $10, %eax    # any sys call # > 10 is invalid
  jg sys_call_error

  # go into jump table
  call *sys_call_table(, %eax, 4)
  jmp sys_call_done

sys_call_error:
  movl $-1, %eax
  
sys_call_done:
  popl %ebx
  popl %ecx
  popl %edx
  iret

/* set up in syscall.c */
.align 32
sys_call_table:
  .rept 10
  .long 0
  .endr
