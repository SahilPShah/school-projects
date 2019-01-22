#include "idt.h"
#include "x86_desc.h"

/*
*Description: initializes the IDT
*Input: none
*Output: none
*Return: none
*Side effecs: loads processor exceptions and keyboard/rtc interrupt handlers into IDT
*/
void idt_init(){
  int i;
  /*the first 32 entries in the IDT are reserved for processor exceptions
  *
  */
  for(i = 0; i < EXC; i++){
    idt[i].seg_selector = KERNEL_CS;
    idt[i].reserved4 = 0;
    idt[i].reserved3 = 1;   // trap gate
    idt[i].reserved2 = 1;
    idt[i].reserved1 = 1;
    idt[i].size = 1;
    idt[i].reserved0 = 0;
    idt[i].dpl = 0;
    idt[i].present = 1;
    if(i > 19)
      idt[i].present = 0;
  }
  for(i = EXC; i < NUM_VEC; i++){
    idt[i].seg_selector = KERNEL_CS;
    idt[i].reserved4 = 0;
    idt[i].reserved3 = 0;   // interrupt gate
    idt[i].reserved2 = 1;
    idt[i].reserved1 = 1;
    idt[i].size = 1;
    idt[i].reserved0 = 0;
    idt[i].dpl = 0;
    idt[i].present = 0;
  }
  
/*
*Each call to SET_IDT_ENTRY, sets the function handler pointer to the specified location in the IDT
*when an interrupt or exception is triggered, the corresponding handler will be exectuted using the handlers function
*pointer
*/
SET_IDT_ENTRY(idt[0], div_asm);
SET_IDT_ENTRY(idt[1], debug_asm);
SET_IDT_ENTRY(idt[2], nmi_asm);
SET_IDT_ENTRY(idt[3], breakpoint_asm);
SET_IDT_ENTRY(idt[4], overflow_asm);
SET_IDT_ENTRY(idt[5], range_bound_asm);
SET_IDT_ENTRY(idt[6], invalid_opcode_asm);
SET_IDT_ENTRY(idt[7], device_not_avail_asm);
SET_IDT_ENTRY(idt[8], double_fault_asm);
SET_IDT_ENTRY(idt[9], coprocessor_segment_overrun_asm);
SET_IDT_ENTRY(idt[10], invalid_tss_asm);
SET_IDT_ENTRY(idt[11], segment_not_present_asm);
SET_IDT_ENTRY(idt[12], stack_segment_fault_asm);
SET_IDT_ENTRY(idt[13], general_protection_fault_asm);
SET_IDT_ENTRY(idt[14], page_fault_asm);
idt[15].present = 0;      //reserved in the IDT
SET_IDT_ENTRY(idt[16], floating_point_exc_asm);
SET_IDT_ENTRY(idt[17], alignment_check_asm);
SET_IDT_ENTRY(idt[18], machine_check_asm);
SET_IDT_ENTRY(idt[19], simd_asm);

SET_IDT_ENTRY(idt[PIT_ENTRY], pit_asm);
idt[PIT_ENTRY].present = 1;  //set the present bit to one
SET_IDT_ENTRY(idt[KBD_ENTRY], kbd_asm);
idt[KBD_ENTRY].present = 1;  //set the present bit to one
SET_IDT_ENTRY(idt[RTC_ENTRY], rtc_asm);
idt[RTC_ENTRY].present = 1;  //set the present bit to one

/*
*populate table for system calls
*/
SET_IDT_ENTRY(idt[SYSCALL_ENTRY], sys_call_asm);
idt[SYSCALL_ENTRY].present = 1;
idt[SYSCALL_ENTRY].reserved3 = 1;   // trap gate
idt[SYSCALL_ENTRY].dpl = 3;
}
