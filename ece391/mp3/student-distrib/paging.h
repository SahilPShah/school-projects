/* init for paging */
#ifndef _PAGING_H
#define _PAGING_H

#include "x86_desc.h"
#include "lib.h"

#define KERNELADDR  0x00400000
#define VMADDR      0x000B8000

#define TERM_FENCE	0x00003000			  			// Space between terminal VM addresses

// virtual addresses for terminals, is the same for physical
#define TERM_VADDR_IDX_DIVIDER   12			// shifts to turn the vaddr into an index
#define PAGE_DIR_IDX_DIVIDER     22

#define TERM_VADDR_BASE			 0x02400000
#define TERM1_VADDR              0x02400000		// Virtual addresses, not used in this file but for reference
#define TERM2_VADDR              (TERM1_VADDR+TERM_FENCE)
#define TERM3_VADDR              (TERM1_VADDR+2*TERM_FENCE)

#define TERM1_IDX   ((TERM1_VADDR-TERM_VADDR_BASE)>>TERM_VADDR_IDX_DIVIDER)
#define TERM2_IDX   ((TERM2_VADDR-TERM_VADDR_BASE)>>TERM_VADDR_IDX_DIVIDER)
#define TERM3_IDX   ((TERM3_VADDR-TERM_VADDR_BASE)>>TERM_VADDR_IDX_DIVIDER)

//bit 0
#define PRESENT 	       0x01

//bit 1
#define RW			       0x02

//bit 2
#define SUPERVISOR         0x00
#define USER               0x04

//bit 7
#define POINTStoTABLE 	   0x0             //indicates 4kB page
#define POINTStoPAGE       0x80            //indicated 4MB page

//index into page table
#define VMPTIDX_K  		   0x0B8
#define NUMENTRIES 		   1024
#define SUM 			   4096

void paging_init();
int reset_terminal_paging(int index, int to_default);

uint32_t pagedirectory[NUMENTRIES] __attribute__ ((aligned (SUM)));    // entry 0 corresponds to kernel, entry 1 corresponds to video memory, 2 holds the 3 terminals
uint32_t VMpagetable_k[NUMENTRIES] __attribute__ ((aligned (SUM)));
uint32_t VMpagetable_u[NUMENTRIES] __attribute__ ((aligned (SUM)));
uint32_t VMterminals[NUMENTRIES] __attribute__ ((aligned(SUM)));		// entry TERM1_IDX is for terminal 1, TERM2_IDX for terminal2, etc.

void flush_tlb_2(void);

#endif
