#define ASM     1

.globl test_execute_asm
.globl shell

.text

shell: .string "shell 0"

test_execute_asm:
  pushl %ebp
  movl %esp, %ebp
  movl $2, %eax
  movl $shell, %ebx

  int $0x80  # call page fault
  leave
  ret
