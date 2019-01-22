#ifndef _SYSCALL_H
#define _SYSCALL_H

#include "terminal.h"
#include "rtc.h"
#include "paging.h"
#include "types.h"
#include "lib.h"
#include "exceptions.h"
#include "../syscalls/ece391sysnum.h"

#define FILE_DESCRIPTOR_TABLE_SIZE 	8  // max num. of files allowed to be opened for each process
#define FILE_DESCRIPTOR_TABLE_MIN   2  // first 2 are reserved for stdin and stdout
#define FILE_DESCRIPTOR_TABLE_MASK_MIN	0x3 // bits 0 and 1 are reserved for stdin and stdout (bitmap)
#define FILE_DESCRIPTOR_TABLE_MASK_MAX	0xFF // all file desriptors in use

#define STDIN_FD 			0           // standard file descriptor indices
#define STDOUT_FD 		1

#define FLAG_OPEN			1           // for checking if an fd is in use or not
#define FLAG_CLOSED		0

#define MAX_ALLOWED_PID  5        // only 6 processes are allowed (starting from 0)
#define NUM_TERMINALS    3

#define RTC_TYPE						0     // to differentiate between file types in each fd (stored in flags)
#define DIR_TYPE 						2
#define FILE_TYPE						4
#define STDINOUT_TYPE				6
#define FD_MAX_NAME_LENGTH	32    // longest file name allowed by file system
#define MAX_ARG_LENGTH      128

#define KB_8			0x00002000      // addresses in virtual memory
#define MB_8 			0x00800000
#define MB_4 			0x00400000
#define MB_128    0x08000000
#define MB_132    0x08400000
#define PROGIMG 	0x08048000
#define MB_128_PD 				32      // index in page directory for 4 MB page to allocate to process file contents

#define VMAP_VMA_T0   0x08500000    // T0 virtual addr = 133 MB
                                    // T1 virtual addr = 133 MB + 4 KB
                                    // T2 virtual addr = 133 MB + 8 KB
#define VMAP_PD           33
#define VMPTIDX_T0       256        // for T1, use 257; for T2, use 258

#define DATABLOCKSIZE 	4096      // size of data block in file system (in B)
#define KB              1024      // 1024 B = 1 kB

//Used to change the return values, if enabled returns values fom -1 to -5, else its just -1
#define SYS_DBG						0

int8_t active[MAX_ALLOWED_PID + 1];
int32_t current_pid;            // global variables: index of currently running process and number of running processes
uint32_t proc_count;

typedef struct file_operations_table {
  uint32_t read_func;    // jump table for each device's specific functions
  uint32_t write_func;           // file's inode numer in file system (used for reading)
  uint32_t open_func;       // current position in the file (used for reading)
  uint32_t close_func;				      // currently only have active and inactive (1 = open, 0 = close)
} fops_t;

typedef struct file_descriptor_block {
  fops_t* fops_table;           // jump table for each device's specific functions
  uint32_t inode_num;           // file's inode numer in file system (used for reading)
  uint32_t file_position;       // current position in the file (used for reading)
  uint32_t flags; 				      // currently only have active and inactive (1 = open, 0 = close)
} fd_t;

typedef struct process_control_block {
	fd_t fd_arr[FILE_DESCRIPTOR_TABLE_SIZE]; // file descriptor array to keep track of (8 fds may be open at one time)
	uint32_t pid;                 // process ID
  uint32_t parent_pid;          // parent's process ID
	uint32_t ret_sp;              // ESP and EBP to restore upon halt system call
  uint32_t ret_bp;
  uint8_t open_files;		        // BITMASK for open file descriptors
  char proc_name[FD_MAX_NAME_LENGTH + 1];      // name of the process ("shell", "hello", etc.)
  char arg[MAX_ARG_LENGTH];
} pcb_t;

//flush the TLB
void flush_tlb();
// converting between pid and the pointer to its pcb
pcb_t* pid2pcb_addr(uint32_t pid);
uint32_t pcb_addr2pid(pcb_t* current_pcb);
// initializes the system call jump table
void sys_call_init();
// initializes the pcb structure for a given process name and current process ID
uint32_t pcb_init(pcb_t* new_pcb, uint8_t* proc_name);
// halt system call (ends the currently running process)
int32_t halt(uint8_t status);
// execute system call (begins execution of a new process after setting up some data)
int32_t execute(const uint8_t* command);
// read system call (reads from a file in file system or a device)
int32_t read(int32_t fd, void* buf, int32_t nbytes);
// write system call (can only write to terminal and rtc - read only file system)
int32_t write(int32_t fd, const void* buf, int32_t nbytes);
// open system call (fills a file descriptor block for a file opened by a process)
int32_t open(const uint8_t* filename);
// close system call (removes file descriptor from a process's PCB)
int32_t close(int32_t fd);
// retrieve command line arguments
int32_t getargs(uint8_t* buf, uint32_t nbytes);
// give user access to video memory
int32_t vidmap(uint8_t** screen_start);

int32_t set_handler(int32_t signum, void* handler_address);
int32_t sigreturn(void);

// function used by exception handlers to return control to parent process
void return_from_halt(void);

#include "filesys.h"
#endif
