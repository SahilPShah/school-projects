#include "scheduling.h"


/*
*Description: This function initializes three terminals and boots a shell into each of them
*Input: none
*Output: none
*Return: none
*SFX: shell executed two times. The first time is done in kernel.c
*/
void init_terminals(){
	cli();
	terminal_data[current_term].ss0 = tss.ss0;
	terminal_data[current_term].esp0 = tss.esp0;
	terminal_data[current_term].active_pid = terms_init;

	terms_init++;

	update_current_term(terms_init);
 	enable_pit_irq_helper();
	execute((uint8_t*)"shell");
}

/*
*Description: scheduler that swithes between 3 active processes using a round-robin algorithm
*Input: The esp and ebp of the current process
*Output: none
*Return: The next "active terminal" number. The active terminal refers to the terminal that the current process is running on
*SFX: Re-map process paging and changes the active terminal
*/

uint8_t schedule(uint32_t esp, uint32_t ebp) {
	//save current register values. The current process's GPR will be stored above its IRET context on its kernel stack
	uint32_t flags;
	cli_and_save(flags);

	terms_init = NUM_TERMINALS;
	terminal_data[current_term].esp = esp;
	terminal_data[current_term].ebp = ebp;
	terminal_data[current_term].ss0 = tss.ss0;
	terminal_data[current_term].esp0 = tss.esp0;
	terminal_data[current_term].active_pid = current_pid;

	// get the current and next pid
	uint8_t next_terminal = (current_term+1)%3;
	uint32_t next_pid = terminal_data[next_terminal].active_pid;
	current_pid = next_pid;

	//setup new process paging
  flush_tlb();
	int physaddr = MB_8 + (next_pid * MB_4);		//virtual address of process
	pagedirectory[MB_128_PD] = physaddr + POINTStoPAGE + USER + RW + PRESENT;

	//put new process info into TSS
	tss.ss0 = terminal_data[next_terminal].ss0;
	tss.esp0 = terminal_data[next_terminal].esp0;				//8MB - (pid * 8KB) - 4

	if(terminal_data[next_terminal].rtc_freq != 0 && (terminal_data[next_terminal].rtc_freq != terminal_data[current_term].rtc_freq))
		rtc_write(terminal_data[next_terminal].rtc_freq);

	update_current_term(next_terminal);			//swtiches the terminal that write functions will write to...name changed to update_current_term

	restore_flags(flags);
	return next_terminal;
}
