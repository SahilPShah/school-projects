#include "paging.h"
/*
*Description: initializes paging
*Input: none
*Output: none
*Return: none
*Side effecs: intializes the page directory with entires for the kernal and video memory spaces
							initializes the video memory page in the page table
*/
void paging_init(){
	uint32_t flags;
	cli_and_save(flags);
	int i;
	for(i = 0; i < NUMENTRIES; i++){
		pagedirectory[i] = 0;
		VMpagetable_k[i] = 0;
		VMpagetable_u[i] = 0;
		//VMterminals[i] = 0;
	}
	//need to set address, page size, user/supervisor, present
	pagedirectory[0] = ((uint32_t)VMpagetable_k) + POINTStoTABLE + SUPERVISOR + RW + PRESENT; 
	pagedirectory[1] = KERNELADDR + POINTStoPAGE + SUPERVISOR + RW + PRESENT;
	pagedirectory[(TERM_VADDR_BASE >> PAGE_DIR_IDX_DIVIDER)] = ((uint32_t)VMterminals) + POINTStoTABLE + SUPERVISOR + RW + PRESENT;
	
	VMpagetable_k[VMPTIDX_K] = VMADDR + SUPERVISOR + RW + PRESENT; 
	VMterminals[TERM1_IDX] = TERM1_VADDR + USER + RW + PRESENT;
	VMterminals[TERM2_IDX] = TERM2_VADDR + USER + RW + PRESENT;
	VMterminals[TERM3_IDX] = TERM3_VADDR + USER + RW + PRESENT;

	//inline assembly for paging enable
	asm volatile ("							   \n\
		movl %0, %%eax  					   \n\
		movl %%eax, %%cr3						 \n\
		movl %%cr4, %%eax            \n\
		orl $0x00000010, %%eax       \n\
		movl %%eax, %%cr4            \n\
		movl %%cr0, %%eax            \n\
		orl $0x80000001, %%eax       \n\
		movl %%eax, %%cr0            \n\
	"
		: // output operands
		: "r" (pagedirectory)		//inputs
		: "eax"// clobbers
	);
	restore_flags(flags);
}

/* 
in reset_terminal_paging(int index, int to_default)

Description: modifies the VMterminals paging table to redirect pages
inputs: 
	- int index: terminal to change
	- int to_default: 
		0: sets the paging structure to point to video memory
		1: sets the paging structure to point to the reserved memory for that terminal  
*/
int reset_terminal_paging(int index, int to_default){
	int table_index;
	int vaddr_base;
	int flags;
	cli_and_save(flags);

	// Determine the index and virtual address values
	switch(index){
		case 0:
			table_index = TERM1_IDX;
			vaddr_base = TERM1_VADDR;
			break;
		case 1: 
			table_index = TERM2_IDX;
			vaddr_base = TERM2_VADDR;
			break;
		case 2:
			table_index = TERM3_IDX;
			vaddr_base = TERM3_VADDR;
			break;
		default:
			return -1;
	}

	flush_tlb_2();
	// edit the table based on to_default
	if(to_default){
		VMterminals[table_index] = vaddr_base + USER + RW + PRESENT;
		restore_flags(flags);
		return 1;
	}

	VMterminals[table_index] = VMADDR + USER + RW + PRESENT;
	restore_flags(flags);
	return 1;
}

/*
desc: flushes the TLB, same as other one
inputs: none
outputs: none
notes: clobbers eax
*/
void flush_tlb_2(void)
{
	asm volatile ("				\n\
		movl %%cr3, %%eax		\n\
		movl %%eax, %%cr3		\n\
		"
	: // no outputs
	: // no inputs
	: "eax" //clobbers
	);
}


