#ifndef _PIT_H
#define _PIT_H

#include "i8259.h"
#include "lib.h"
#include "scheduling.h"
#include "terminal.h"

#define PIT_IRQ_NUM	    0
#define PIT_CH0_PORT    0x40
#define PIT_CMD_PORT    0x43

#define PIT_CMD           0x30
#define PIT_COUNT_HI_SLOW 0xE9
#define PIT_COUNT_LO_SLOW 0x0B
#define PIT_COUNT_HI_FAST 0x2E
#define PIT_COUNT_LO_FAST 0xE0

#define NUM_TERMINALS 3

void pit_set_count();
void pit_init();
void enable_pit_irq_helper();
void pit_handler_wrapper();

uint8_t pit_handler(uint32_t esp, uint32_t ebp);

#endif
