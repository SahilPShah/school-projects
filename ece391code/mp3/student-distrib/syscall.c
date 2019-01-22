#include "x86_desc.h"
#include "syscall.h"
#include "pit.h"

/* Private Syscall helper functions*/
void parse(const uint8_t* command, uint8_t* fn, uint8_t* args);
int32_t get_new_pid();

/* helper function for debugging. returns specefiic number (specified) if debugging is turned on, else -1
e.g. >>>> return fail_ret(-3);*/

int32_t fail_ret(int num){
	#if SYS_DBG
	return num;
	#endif
	return -1;
}

/*
*Description: initialize the system call jump table with function pointers to each system call handler
*Input: none
*Output: populates the system call table
*Return: none
*SFX: none
*/
void sys_call_init(void){
	int i;

	sys_call_table[SYS_HALT] = (uint32_t)halt;
	sys_call_table[SYS_EXECUTE] = (uint32_t)execute;
	sys_call_table[SYS_READ]= (uint32_t)read;
	sys_call_table[SYS_WRITE] = (uint32_t)write;
	sys_call_table[SYS_OPEN] = (uint32_t)open;
	sys_call_table[SYS_CLOSE] = (uint32_t)close;
	sys_call_table[SYS_GETARGS] = (uint32_t)getargs;
	sys_call_table[SYS_VIDMAP] = (uint32_t)vidmap;
	sys_call_table[SYS_SET_HANDLER] = (uint32_t)set_handler;
	sys_call_table[SYS_SIGRETURN] = (uint32_t)sigreturn;

	for(i = 0; i <= MAX_ALLOWED_PID; i++)
		active[i] = 0;
	current_pid = -1;
	proc_count = 0;
}

/*
*Description: initialializes a pcb
*Inputs: pcb_t* new_pcb, uint8_t* proc_name: the pcb to populate and the name of the process
*Outputs: none
*Return: process ID of the new pcb
SFX: initializes the PCB
*/
uint32_t pcb_init(pcb_t* new_pcb, uint8_t* proc_name) {
	// clear all tables
	int i;
	for (i = 0; i < FILE_DESCRIPTOR_TABLE_SIZE; i ++){
		uint32_t fops_addr = (uint32_t)(new_pcb) + sizeof(pcb_t) + (i * sizeof(fops_t));
		new_pcb->fd_arr[i].fops_table = (fops_t*)(fops_addr);
		new_pcb->fd_arr[i].fops_table->read_func = 0;
		new_pcb->fd_arr[i].fops_table->write_func = 0;
		new_pcb->fd_arr[i].fops_table->open_func = 0;
		new_pcb->fd_arr[i].fops_table->close_func = 0;
		new_pcb->fd_arr[i].inode_num = 0;
		new_pcb->fd_arr[i].file_position = 0;
		new_pcb->fd_arr[i].flags = FLAG_CLOSED;
	}

	//open STDIN
	new_pcb->fd_arr[STDIN_FD].flags = FLAG_OPEN + STDINOUT_TYPE;
	new_pcb->fd_arr[STDIN_FD].fops_table->read_func = (uint32_t)terminal_read;

	//open stdout
	new_pcb->fd_arr[STDOUT_FD].flags = FLAG_OPEN + STDINOUT_TYPE;
	new_pcb->fd_arr[STDOUT_FD].fops_table->write_func = (uint32_t)terminal_write;

	// == 0x3 for sysin and sysout (0 and 1 bit)
	new_pcb->open_files = FILE_DESCRIPTOR_TABLE_MASK_MIN;

	new_pcb->pid = get_new_pid();

	uint32_t length = strlen((int8_t*)proc_name);
	strncpy(new_pcb->proc_name, (int8_t*)(proc_name), length);
	for(i = 0; i < MAX_ARG_LENGTH; i++)
		new_pcb->arg[i] = 0;
	if(terms_init < NUM_TERMINALS || terminal_data[displayed_term].active_pid == 2 * NUM_TERMINALS)
		new_pcb->parent_pid = -1;
	else
		new_pcb->parent_pid = terminal_data[displayed_term].active_pid;
	new_pcb->ret_sp = 0;
	new_pcb->ret_bp = 0;

	return new_pcb->pid;
}


/*
*Description: get the first inactive PID to assign to the next upcoming 
*Input: none
*Output: none
*Return: none
*SFX: none
*/
int32_t get_new_pid() {
	int i;
	for(i = 0; i <= MAX_ALLOWED_PID; i++) {
		if(active[i] == 0)
			return i;
	}
	return -1;
}

/*
*Description: system call that will execute a user level program. This syscall parses the command from the shell command line, verifies the file
*			  is an executable file. Then is creates the user programs page in virtual memory. Finally, the file is read to get the entry point
*			  of the program and the SS and the new ESP are stored in the TSS. Once this pre-processing is complete, context switching can occur.
*Input: uint8_t* command: string command from the shell
*Output: executes the user level program
*Return: -1 if file cannot be executed or new process cannot be created. return 256 if halt because of exception. return 0-255 if normal halt
*SFX: will switch processes and modifies the TSS
*/
int32_t execute(const uint8_t* command) {
	//code to parse command to get args
	cli();
	if(current_pid >= MAX_ALLOWED_PID || command == NULL || get_new_pid() == -1) {
		printf("Maximum number of processes reached, aborting...\n");
		return 1;
	}

	int i;
	pcb_t* pcb = (pcb_t*)(MB_8 - (get_new_pid() + 1) * KB_8);
	uint8_t filename[33];
	uint8_t args[MAX_ARG_LENGTH];
	for(i = 0; i < MAX_ARG_LENGTH; i++)
		args[i] = 0;
	parse(command, filename, args);		// need to parse commands and store in pcb, getargs copies these commands into user space

	dentry_t dentry;
	uint8_t buf[4] = {0,0,0,0};			//buffer to read first four bytes from file
	uint8_t inst_addr[4];
	uint8_t filedata[DATABLOCKSIZE];
	uint32_t progstart = 0;
	uint32_t ret_val;

	if(read_dentry_by_name(filename, &dentry)){
		return -1;				//the filename cannot be found
	}
	else{
		if(read_data(dentry.inode_num, 0, buf, 4) != 4)
			return -1;
		if(!(buf[0] == 0x7F && buf[1] == 0x45 && buf[2] == 0x4C && buf[3] == 0x46))		// magic numbers for executable check
			return -1;	//if not an executable
	}
	if(read_data(dentry.inode_num, 24, inst_addr, 4) != 4)
		return -1;
	//convert the byte array to a 32 bit number
	for(i = 0; i < 4; i++){
		progstart+=(inst_addr[i] << (i*8));
	}

	uint32_t new_pid = pcb_init(pcb, filename);
	active[new_pid] = 1;
	memcpy((uint8_t*)(pcb->arg), args, strlen((char*)args));

	//HOW TO GET THE PROCESS ID (process number)
	int physaddr = MB_8 + (new_pid * MB_4);		//virtual address of process
	pagedirectory[MB_128_PD] = physaddr + POINTStoPAGE + USER + RW + PRESENT;

	//flush TLB here
	flush_tlb();

	//read executable data
	uint32_t file_length = sc_get_file_size(dentry.inode_num);
	uint32_t total_bytes = file_length;
	for(i = 0; i < (file_length/DATABLOCKSIZE); i++){
		if(read_data(dentry.inode_num, (i * DATABLOCKSIZE), filedata, DATABLOCKSIZE) != DATABLOCKSIZE)
			return -1;
		memcpy((uint8_t*)(PROGIMG + i * DATABLOCKSIZE), filedata, DATABLOCKSIZE);
		total_bytes-=DATABLOCKSIZE;
	}
	if(read_data(dentry.inode_num, (i * DATABLOCKSIZE), filedata, total_bytes) != total_bytes)
		return -1;
	//load into memory
	memcpy((uint8_t*)(PROGIMG + i * DATABLOCKSIZE), filedata, total_bytes);

	current_pid = new_pid;
	proc_count++;
	terminal_data[current_term].active_pid = current_pid;

	//put new process info into TSS
	tss.ss0 = KERNEL_DS;
	tss.esp0 = MB_8 - ((new_pid)*KB*8) - 4;				//8MB - (pid * 8KB) - 4

	// push return_from_halt, which will act like a return address if halt is called
	// then save the stack pointer and base pointer into the pcb, so you can restore it in halt
 	asm volatile ("				\n\
		pushl %2		      	\n\
		movl %%esp, %0		  \n\
		movl %%ebp, %1			\n\
		"
		: "=r"(pcb->ret_sp), "=r"(pcb->ret_bp)
		: "r"(&&return_from_halt)
		: "eax"
	);
	// setup for iret. All these values that are pushed are popped by iret
	asm volatile(	// need to push the new SS and ESP need to store new process info into TSS (esp0)
		"pushl %3; " 	  // new SS
		"pushl %1; "
		"pushfl; "            // push eflags
		"popl %%ebx; "           // pop flags into eax
		"orl $0x0200, %%ebx; "   // set interrupt
		"orl $0x3000, %%ebx; "    // set priviledge level to 3
		"pushl %%ebx; "
		"pushl %2; "       // new CS
		"pushl %0; "
		"iret; "
		: //no outputs
		:"r"(progstart), "r" (MB_132-4), "r"(USER_CS), "r"(USER_DS)
		:"ebx"
	);

	// returns here after halt through a ret command
    return_from_halt:

	// if it was halted from an exception, return 256
	if (exceptions_flag){
		exceptions_flag = 0;
		sti();
		return 256;
	}

	// moves %eax into ret_val
    asm volatile("movl %%eax, %0;"
            :"=r" (ret_val)
            :
            :"eax"
           );
	sti();
    return ret_val;
}

/*
*Description: terminates a process and returns status to execute, which returns that value. This function restores the state of the parent process and its necessary
*			  registers.
*Input: uint8_t status: this value should be returned by the execute system call
*Output: returns to parent process
*Return: status : should never actually return a value because it should jump to execute before reaching the end of control
SFX: modifies the TSS
*/
int32_t halt(uint8_t status){
	/*switch to kernel stack, the stack that is saved at the start of execute
	 modify the stack so you can iret
	iret*/
	cli();

	if(((pid2pcb_addr(current_pid))->parent_pid == -1) && (!strncmp((pid2pcb_addr(current_pid))->proc_name, "shell", 5))) {
		if(exceptions_flag)
			printf("program terminated by exception\n");
		printf("Halt called on a base shell, restarting...\n");
		proc_count--;
		active[current_pid] = 0;
		terminal_data[current_term].active_pid = 2 * NUM_TERMINALS;
		execute((uint8_t*)"shell");
		return 0;
	}

	// close all FD's
	int x;
	for (x = 0; x < FILE_DESCRIPTOR_TABLE_SIZE; x ++){
		close(x);
	}

	uint32_t par_pid = (pid2pcb_addr(current_pid))->parent_pid;

	flush_tlb();

	uint32_t physaddr = MB_8 + (par_pid*MB_4);
	pagedirectory[MB_128_PD] = physaddr + POINTStoPAGE + USER + RW + PRESENT;

	//enter in the return value, then jmp
	uint32_t ret_val = (uint32_t)status;

	tss.ss0 = KERNEL_DS;				// need to store the SS0 for each process in execute then restore it in halt
	tss.esp0 = MB_8 - ((par_pid)*KB*8) - 4;// old stack pointer, is this the correct thing to put in tss?

	pcb_t* pcb = pid2pcb_addr(current_pid);
	proc_count--;
	active[current_pid] = 0;
	terminal_data[current_term].active_pid = par_pid;
	current_pid = par_pid;

	// pull the stack pointer and base pointer from the pcb which points to where it was in the execute function
	// The top value is the return_from_halt label which was pushed in execute, so ret jumps to that
	asm volatile ("	movl %0, %%esp; "
		"movl %1, %%ebp;"
		"movl %2, %%eax;"
		"ret"
		: // no outputs
		: "r" (pcb->ret_sp), "r"(pcb->ret_bp), "r" (ret_val)
		: "eax"
	);
	// should not reach this point
	return ret_val;
}

/*
*Description: helper function to parse entire command
*Input: uint8_t* command: the command arg passed into the execute function
*Output: none
*Return: the filename and arguements parsed from the command passed as a pointer
*SFX: none
*/
void parse(const uint8_t* command, uint8_t* fn, uint8_t* args){
	//get the filename
	int i = 0;
	while(command[i] != ' ' && i < FD_MAX_NAME_LENGTH){
		fn[i] = command[i];
		i++;
	}
	fn[i] = '\0';			//force the filename to be null terminated

	while(command[i] == ' ' && i < FD_MAX_NAME_LENGTH){i++;}		//find the first non space after the filename
	int cmd_start = i;

	//get the args
	while(command[i] != 0 && command[i] != ENTER){
		args[i - cmd_start] = command[i];
		i++;
	}
}

/*
*Description: helper function that flushes the tlb
*Input: None
*Output: None
*Return: None
*SFX: clears the TLB
*/
void flush_tlb(){
	asm volatile ("				\n\
		movl %%cr3, %%eax		\n\
		movl %%eax, %%cr3		\n\
		"
	: // no outputs
	: // no inputs
	: "eax" //clobbers
	);
}

/*
*Description: based on the PID this fucntion will return the address of the corresponding PCB
*Input: uiunt_t pid: the procesd ID
*Output: none
*Return: pcb_t*: the address of pcb of the requested process
*SFX: none
*/
pcb_t* pid2pcb_addr(uint32_t pid) {
	return (pcb_t*)(MB_8 - (current_pid + 1) * KB_8);
}

/*
*Description: based on the PCb this fucntion will return the process ID
*Input: pcb_t* pcb: the pcb
*Output: none
*Return: pcb_t*: the address of pid of the requested pcb
*SFX: none
*/
uint32_t pcb_addr2pid(pcb_t* pcb) {
	return ((MB_8 - (uint32_t)pcb) / KB_8) - 1;
}

/*
*Description: data should be read from the keyboard, a file, deveice, or directory and returns the number of bytes read.
			  While reading, this function will update its position into a file/directory.
*Input: int32_t fd, void* buf, int32_t nbytes: the file descriptor index, the buffer to read into and the number of bytes
		to read
*Output: changes the position inside a file or directory
*Return: number of bytes read
*SFX: none
*/
int32_t read(int32_t fd, void* buf, int32_t nbytes){
	pcb_t* pcb = pid2pcb_addr(current_pid);

	// check for invalid use cases
	if (fd < 0 || (pcb->fd_arr[fd].flags & 1) == FLAG_CLOSED || buf == NULL || nbytes == 0){
		return fail_ret(-6);
	}

	// check if fd has a read function
	uint32_t read_fnc = pcb->fd_arr[fd].fops_table->read_func;
	if (read_fnc == 0){
		return fail_ret(-2);
	}

	int x;
	for(x = 0; x < nbytes; x++){
		((uint8_t*)(buf))[x] = 0;
	}

	// call the specific read function from the jump table and store number of byes read
	int bytes_read = ((int (*)(uint32_t, void*, int32_t, uint32_t))(read_fnc))(pcb->fd_arr[fd].inode_num, buf, nbytes, pcb->fd_arr[fd].file_position);

	// update file position
	if ((pcb->fd_arr[fd].flags & 6) == FILE_TYPE)
		pcb->fd_arr[fd].file_position += bytes_read;
	else if((pcb->fd_arr[fd].flags & 6) == DIR_TYPE)
		pcb->fd_arr[fd].file_position++;
	return bytes_read;
}

/* write
 * DESCRIPTION: System call used to write data to a file or device
 * INPUT: fd (file descriptor: number returned from open)
 					buf (probably a char* - pointer to array that stores chars to write)
					nbytes (number of bytes to print from buf)
 * OUTPUT: number of bytes actually written on success (0 - nbytes) or -1 on failure (error occured - cannot write)
 * SIDE EFFECT: Changes appearance of terminal (prints to it) if stdout is used
 								Changes interrupt rate of RTC if file type is 0 (represents RTC device "file")
 */
int32_t write(int32_t fd, const void* buf, int32_t nbytes){
	pcb_t* pcb = pid2pcb_addr(current_pid);		// retrieve the address of the current process's PCB

	// invalid use cases (negative fd, closed fd, invalid buffer address, number of bytes to write = 0)
	if (fd < 0 || (pcb->fd_arr[fd].flags & 1) == FLAG_CLOSED || buf == NULL || nbytes == 0){
		return -1;
	}

	uint32_t write_fnc = pcb->fd_arr[fd].fops_table->write_func; // retrieve device specific write function

	if (write_fnc == 0){	// file and dir and stdin can't write
		return fail_ret(-3);
	}

	// RTC takes a different format, only takes 1 int32_t (nbytes auto defaults to 1)
	// call rtc_write explicitly to simplify arguments
	if ((pcb->fd_arr[fd].flags & 6) == RTC_TYPE) {
		// rate is a 4 byte value
		int ret_val = ((int (*)(uint32_t))(write_fnc))(*(uint32_t*)buf);
		if(!ret_val)			// successful rtc write
			terminal_data[current_term].rtc_freq = (*(uint32_t*)buf);
		return ret_val;
	}

	// All other writes should be in the form of (buf*, bytes) -> call the write function after pushing args
	// and write output to bytes_written
	int bytes_written = ((int (*)(const void*, int32_t))(write_fnc))(buf, nbytes);
	return bytes_written;
}

/* open
 * DESCRIPTION: System call used to open/"allocate" a file descriptor in a PCB's file descriptor array
 								Creates an entry in the file descriptor table
 * INPUT: filename (name of the file in the file system to create a file descriptor for)
 * OUTPUT: newly opened fd index on success or -1 on failure (fd could not be allocated)
 * SIDE EFFECT: PCB's open files bitmask is changed to represent the now open file descriptor
 								If the file to open is for RTC, the default interrupt rate (2Hz) will be set on the RTC
 */
int32_t open(const uint8_t* filename){
	pcb_t* pcb = pid2pcb_addr(current_pid);		// retrieve the address of the current process's PCB

	// check to see if max number of files are already open
	if (pcb->open_files == FILE_DESCRIPTOR_TABLE_MASK_MAX){
		return -1;
	}

	// check to see if this file is already open
	int i;

	// find next available file_descriptor
	int fd_num = 2;
	for (i = 0x4; i <= FILE_DESCRIPTOR_TABLE_MASK_MAX; i *= 2){		// start search at bit 2, end search at bit 7
		if (!(pcb->open_files & i)){			// found an unopened fd index
			break;
		}
		fd_num++;
	}

	if (fd_num == 8){		// critical error (all fd indices filled, but open files bitmap doesn't show this)
		return fail_ret(-3);
	}

	// attempt to fill the fd block with relevant data
	if (fill_fd_block_by_name(filename, &(pcb->fd_arr[fd_num])) != 0) {
		return fail_ret(-4);
	}
	// get the device specific open function from the jump table (populated by fill_fd_block_by_name)
	uint32_t open_fnc = pcb->fd_arr[fd_num].fops_table->open_func;

	// call the open function - should return 0
	if(((int (*)(void))(open_fnc))()){
		return -1;
	}

	// mark the fd as open, and update bit mask
	pcb->fd_arr[fd_num].flags += FLAG_OPEN;
	pcb->open_files |= i;
	return fd_num;
}

/* close
 * DESCRIPTION: System call used to remove/"deallocate" a file descriptor from a PCB's file descriptor array
 								Checks to see if the fd can be closed and closes it. Clears values in the descriptor table.
 * INPUT: fd (file descriptor to remove from array)
 * OUTPUT: 0 on success (fd removed) or -1 on failure (fd could not be removed)
 * SIDE EFFECT: PCB's open files bitmask is changed to represent the now closed file descriptor
 */
int32_t close(int32_t fd){
	pcb_t* pcb = pid2pcb_addr(current_pid);		// retrieve the address of the current process's PCB

    // can't close these (stdin, stdout, or a negative fd)
	if (fd < 0 || fd == STDIN_FD || fd == STDOUT_FD){

		return fail_ret(-6);
	}

    // if the fd is already closed, must return -1
	if((pcb->fd_arr[fd].flags & 1) == FLAG_CLOSED){

		return fail_ret(-2);
	}

	// get device specific close function from the jump table
	uint32_t close_fnc = pcb->fd_arr[fd].fops_table->close_func;

	// call the device's close function - should return 0
	if(((int (*)(void))(close_fnc))()){
		return -1;
	}


	// clear PCB and file descriptor information
	pcb->fd_arr[fd].inode_num = 0;
	pcb->fd_arr[fd].file_position = 0;
	pcb->fd_arr[fd].flags = FLAG_CLOSED;

	// clear the jump table
	pcb->fd_arr[fd].fops_table->read_func = 0;
	pcb->fd_arr[fd].fops_table->write_func = 0;
	pcb->fd_arr[fd].fops_table->open_func = 0;
	pcb->fd_arr[fd].fops_table->close_func = 0;

	// update the PCB's open files bitmap
	int i = 0x01 << fd;
	pcb->open_files &= ~(i); 		// update open_files mask (change that value to zero)
	return 0;
}

/*
*Description: this system call extracts the arguements of the current process
*Inputs: uint8_t* buf, uint32_t nbytes: the buffer to copy the arguements into and the number of bytes to copy
*Outputs: none
*Return: returns 0 on success and -1 on fail
*SFX: copies the arguements from the pcb into the buffer
*/
int32_t getargs(uint8_t* buf, uint32_t nbytes) {
	pcb_t* pcb = pid2pcb_addr(current_pid);		// retrieve the address of the current process's PCB

	if(buf == NULL || nbytes == 0 || strlen(pcb->arg) >= nbytes || pcb->arg[0] == 0){
		return -1;
	}

	memcpy(buf, pcb->arg, nbytes);		// copy arguments to user space
	return 0;
}

/*
*Description: this system call virtualizes video to allow user level programs to access it
*Inputs: uint8_t** screen_start: a pointer to the virtual address where VM must be set up
*Outputs: none
*Return: returns 133 MB on success and -1 on fail
*SFX: sets up a page table in the page directory and sets up a page in the page table
*/
int32_t vidmap(uint8_t** screen_start) {
	if((uint32_t)screen_start < MB_128 || (uint32_t)screen_start >= MB_132){
		return -1;
	}
	uint32_t flags;
	cli_and_save(flags);
	flush_tlb();				// flush TLB and modify paging structures to give user access to video memory
	pagedirectory[VMAP_PD] = ((uint32_t)VMpagetable_u) + POINTStoTABLE + USER + RW + PRESENT;
	if(current_term == displayed_term)
		VMpagetable_u[VMPTIDX_T0 + current_term] = VMADDR + USER + RW + PRESENT;
	else
		VMpagetable_u[VMPTIDX_T0 + current_term] = (uint32_t)(get_terminal_address(current_term)) + USER + RW + PRESENT;
	*screen_start = (uint8_t*)(VMAP_VMA_T0 + 4 * KB * current_term);			// store 133 MB address in screen_start and return it
	restore_flags(flags);
	return (int32_t)(*screen_start);
}

/********OPTIONAL**********/

int32_t set_handler(int32_t signum, void* handler_address) {
	return -1;
}

int32_t sigreturn(void) {
	return -1;
}
