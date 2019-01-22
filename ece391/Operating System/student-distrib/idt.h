#ifndef _IDT_H
#define _IDT_H

#include "x86_desc.h"
#include "exceptions.h"
#include "keyboard.h"
#define EXC 32
#define PIT_ENTRY 0x20
#define KBD_ENTRY 0x21
#define RTC_ENTRY 0x28
#define SYSCALL_ENTRY 0x80

void idt_init();
extern void sys_call_asm(void);

#endif
