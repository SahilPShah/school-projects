#include "pit.h"

/*
*Description: sets the frequency of the pit
*Input: none
*Output: none
*Return: none
*SFX: none
*/
void pit_set_count() {
  outb(PIT_COUNT_LO_FAST, PIT_CH0_PORT);   /* Set low byte of divisor */
  outb(PIT_COUNT_HI_FAST, PIT_CH0_PORT);     /* Set high byte of divisor */
}

/*
*Description: initialize the PIT
*Input: none
*Output: none
*Return: none
*SFX: none
*/
void pit_init() {
  uint32_t flags;
	cli_and_save(flags);

  outb(PIT_CMD, PIT_CMD_PORT);             /* Set our command byte 0x36 */
  pit_set_count();
  enable_irq(PIT_IRQ_NUM);

  restore_flags(flags);
}

/*
*Description: handles PIT interrupts: Calls the scheduler to switch to another process
*Input: ESP and EBP of the current process
*Output: none
*Return: terminal to swtich to 
*SFX: none
*/
uint8_t pit_handler(uint32_t esp, uint32_t ebp) {
	send_eoi(PIT_IRQ_NUM);
  pit_set_count();
  int x = 0;

  if(terms_init < NUM_TERMINALS - 1){
    terminal_data[terms_init].esp = esp;
    terminal_data[terms_init].ebp = ebp;
    init_terminals();
  }
  else{
    x = schedule(esp, ebp);
  }
  return x;
}

/*
*Description: enable the PIT IRQ
*Input: none
*Output: none
*Return: none
*SFX: none
*/
void enable_pit_irq_helper(){
    enable_irq(PIT_IRQ_NUM);
}

/*
*Description: Save the ESP and EBP values, stored in EAX and EBX, into the terminal struct and calls the PIT handler. 
*Input: none
*Output: none
*Return: none
*SFX: none
*/
void pit_handler_wrapper(){
  int x;
  uint32_t esp;
  uint32_t ebp;

  asm volatile(
    "movl %%eax, %0;"
    "movl %%ebx, %1;"
    : "=r"(esp), "=r"(ebp)
    : //
    : "eax"
  );

  disable_irq(PIT_IRQ_NUM);
  x = pit_handler(esp, ebp);
  enable_irq(PIT_IRQ_NUM);

  asm volatile(
    "movl %0, %%eax;"
    "movl %1, %%ebx;"
    : //
    : "r"(terminal_data[x].esp), "r"(terminal_data[x].ebp)
    : "eax"
  );
}
