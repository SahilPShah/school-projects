#ifndef _SCHEDULING_H
#define _SCHEDULING_H

#include "lib.h"
#include "syscall.h"
#include "terminal.h"
#include "pit.h"

#define NUM_TERMINALS  3

void init_terminals();
uint8_t schedule(uint32_t esp, uint32_t ebp);

#endif
