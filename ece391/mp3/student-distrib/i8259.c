/* i8259.c - Functions to interact with the 8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"
#include "lib.h"

/* Interrupt masks to determine which interrupts are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7  */
uint8_t slave_mask;  /* IRQs 8-15 */


/* void i8259_init(void);
 * Inputs: none
 * Return Value: void
 * Function: Initialize the 8259 PIC */
void i8259_init(void) {
    outb(0xff, MASTER_DATA);         //mask all of the master
    outb(0xff, SLAVE_DATA);          //mask all of the slave

    master_mask = 0xff;
    slave_mask = 0xff;

    outb(ICW1, MASTER_8259_PORT);         //ICW1 --- select master init
    outb(ICW2_MASTER, MASTER_DATA);       //ICW2 --- master mapped from 0x20 to 0x27
    outb(ICW3_MASTER, MASTER_DATA);       //ICW3 --- slave on IR2 (LEAVE HERE OR CHANGE?)
    outb(ICW4, MASTER_DATA);              //ICW4 --- normal EOI

    outb(ICW1, SLAVE_8259_PORT);          //ICW1 --- select slave init
    outb(ICW2_SLAVE, SLAVE_DATA);         //ICW2 --- slave mapped from 0x28-0x2f
    outb(ICW3_SLAVE, SLAVE_DATA);         //ICW3 --- slave is master's IR2
    outb(ICW4, SLAVE_DATA);               //ICW4 --- normal EOI
}

/* void enable_irq(uint32_t irq_num);
 * Inputs: uint32_t irq_num = number of IRQ line to enable interrupts for
 * Return Value: void
 * Function: Enable (unmask) the specified IRQ */
void enable_irq(uint32_t irq_num) {
    uint8_t enable_mask = 1;         // the mask is a bitmap - need to shift the 0 bit to the correct position
    uint8_t shift = 0;               // depending on the irq number (0 means that IRQ line is enabled, 1 means disabled)

    unsigned long flags;
    cli_and_save(flags);

    if(irq_num > MAX_IRQ-1 ) {        //number should be from 0 to 15
			restore_flags(flags);
			return;
		}
    else if(irq_num > IRQ_PER_PIC-1) {        // mask slave pic
        shift = irq_num - IRQ_PER_PIC;        // number of bit shifts to do is IRQ line number - 8 (on slave)
        enable_mask = ~(enable_mask << shift);
        outb((slave_mask & enable_mask), SLAVE_DATA);        // send new mask to PIC and update stored slave mask
        slave_mask = slave_mask & enable_mask;
    }
    else{                                     // mask master pic
        shift = irq_num;                      // number of shifts to do is equal to IRQ line number (on master)
      	enable_mask = ~(enable_mask << shift);
        outb((master_mask & enable_mask), MASTER_DATA);        // send new mask to PIC and update stored master mask
        master_mask = master_mask & enable_mask;
    }
    restore_flags(flags);
}

/* void disable_irq(uint32_t irq_num);
 * Inputs: uint32_t irq_num = number of IRQ line to disable interrupts for
 * Return Value: void
 * Function: Disable (mask) the specified IRQ */
void disable_irq(uint32_t irq_num) {
  unsigned long flags;
  cli_and_save(flags);          // clear interrupt flag and save flags

  if(irq_num > MAX_IRQ - 1) {   //number should be from 0 to 15
    restore_flags(flags);
    return;
  }
  else if(irq_num > IRQ_PER_PIC - 1) {
    uint8_t disable_mask = 0x01;            // the mask is a bitmap - need to shift the 1 bit to the correct position
    uint8_t shift = irq_num - IRQ_PER_PIC;  // number of bit shifts to do is IRQ line number - 8 (on slave)
    disable_mask = disable_mask << shift;   // left shift 1 bit to correct position (not circular because byte is padded with 0)
    outb(slave_mask | disable_mask, SLAVE_8259_PORT + 1);        // send new mask to PIC and update stored slave mask
    slave_mask = slave_mask | disable_mask;
  }
  else {
    uint8_t disable_mask = 0x01;            // the mask is a bitmap - need to shift the 1 bit to the correct position
    uint8_t shift = irq_num;                // number of bit shifts to do is IRQ line number (on master)
    disable_mask = disable_mask << shift;   // left shift 1 bit to correct position (not circular because byte is padded with 0)
    outb(master_mask | disable_mask, MASTER_8259_PORT + 1);        // send new mask to PIC and update stored master mask
    master_mask = master_mask | disable_mask;
  }

  restore_flags(flags);
}

/* void send_eoi(uint32_t irq_num);
 * Inputs: uint32_t irq_num = number of IRQ line to send EOI signal to
 * Return Value: void
 * Function: Send end-of-interrupt signal for the specified IRQ */
void send_eoi(uint32_t irq_num) {
  unsigned long flags;
  cli_and_save(flags);      // clear interrupt flag and save flags

  if(irq_num > MAX_IRQ - 1) {   //number should be from 0 to 15
    restore_flags(flags);
    return;
  }
  else if(irq_num > IRQ_PER_PIC - 1) {      // irq_num is between 8 and 15 -> OR IRQ number with EOI signal and send to slave PIC
    uint8_t slave_num = irq_num - IRQ_PER_PIC;
    outb(EOI | slave_num, SLAVE_8259_PORT);
    outb(EOI | 0x02, MASTER_8259_PORT);
  }
  else                                     // irq_num is between 0 and 7 -> OR IRQ number with EOI signal and send to master PIC
    outb(EOI | irq_num, MASTER_8259_PORT);

  restore_flags(flags);
}
