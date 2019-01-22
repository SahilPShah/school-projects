BUGLOG
=============

### CP1

  Double Fault/Invalid Opcode Exception: This was caused due to an incorrect IDT setup. We attempted to generalize the IDT initialization which    was causing errors for unknown reason. To fix this, we simply hardcorded each IDT entry using                                                    SET_IDT_ENTRY and set the specific IDT struct values for special cases like system calls and interrupts.
  Fixed By: Sahil
  Fixed On: 10/20/2018

  Only one RTC interrupt would occur: This was caused by not reading the C register of the RTC. To fix this, we read the OUTB register in the RTC handler before we re-enable the IRQ line.
  Fixed by: Ryan, Kyle, Sahil
  Fixed On: 10/21/2018

  Boot Looping: The OS would continuously reboot. This was caused by the incorrect paging initialization and the order in which
  the control registers were set. This was fixed by changing the order of the register initialization to the correct order.
  Fixed: Sahil, Ryan
  Fixed On: 10/21/2018

### CP2

  Couldn't read more than 4 kB from any file: Caused by assuming than file data was stored in adjacent data blocks. Fixed by indexing into a file's
  inode array to get correct data block numbers to read correctly.
  Fixed by: Ryan, Kyle
  Fixed On: 10/27/2018

  Unwanted Key Presses still display the unwanted keycodes: When keyboard keys that were meant to be ignored were pressed, they still printed a    character. To fix this, all the unwanted keycodes were translated to the NULL character.
  Fixed By: Ryan
  Fixed On: 11/1/2018

### CP3

  Bug involving circular inclusion of files: Compiler didn't recognize defined functions - it couldn't recognize basic characters. Fixed by checking all files to make sure that they didn't include files that were including them (.h files).
  Fixed by: Sahil, Pat, Ryan, Kyle
  Fixed on: 11/10/2018

  Execute function would not jump to the correct entrypoint of the user level program: The entry point of the program was correctly extracted from the filesystem but was not correctly combined into one address. This bug was caused by incorrect bit shifting when combining the individual bytes from the file. To fix this, (8*Byte_number) is the value that each byte needs to be shifted by
  Fixed by: Sahil
  Fixed On: 11/11/2018

  Halt not working properly: Page fault on the halting of a process. This is was caused by not correctly jumping to the correct location at the end of the execute function. To fix this, we push the desired return address to the stack before pushing the IRET context to the stack. As halt restores the ESP and EBP, the last thing on the stack is the return address to jump to. Calling RET jumps back to that return address.
  Fixed by: Pat
  Fixed On: 11/11/2018

### CP4

  Shell arguments not accepted: This was an issue with the getargs system call, specifically the parse function. The issue was the same index to the arguement in the command buffer was used to index the arguement buffer. To fix this, the length of the string before the arguements was subtracted from the index.
  Fixed by: Ryan
  Fixed On: 11/23/2018

### CP5

  Page fault when trying to jump back to the execution of another process: Initially, we were saving the ESP and EBP in the C handler for the current process and restoring the next process's ESP and EBP at the end of the handler. The issue with this is that the processor modifies the stack when calling a c function which then changes the stack. To fix this, we move the process's ESP and EBP into temporary registers. Then, using a wrapper for the handler, we save the values in those temporary registers into the terminal struct.
  Fixed By: Pat, Sahil, Ryan
  Fixed On: 12/6/2018

  Parent process ID logical errors: Bug caused page faults after attempting to halt processes - parent process IDs for processes running on top of a base shell were being set to -1, not process IDs of the base shell. This problem was fixed by updating the logic for setting the parent PID in pcb_init and in halt (wait for all terminals to be initialized, then no parent PIDs should be set to -1; if halting a base shell, set active PID on displayed terminal to 6 so pcb_init knows it needs to reinitialize a base shell with parent PID of -1).
  Fixed By: Ryan
  Fixed On: 12/8/2018

  Memcpy argument passing bug: Bug resulted in terminal buffer not being cleared, which caused no such command shell return statements when running a program after halting a different one. The bug was fixed by switching the source and destination pointers for the memcpy function calls in terminal read (copy cleared global buffer to local, terminal-specific buffer).
  Fixed By: Ryan
  Fixed On: 12/8/2018

  System call generalization: This was a bug created as a result of one global read_waiting flag being used to allow processes to read from the terminal, which resulted in incorrect read/write behavior on non-displayed terminals. The bug was fixed by creating and storing read_waiting flags for each of the three terminals, which prevented non-displayed terminals from being able to read (can't type onto a non-displayed terminal.
  Fixed By: Ryan
  Fixed On: 12/8/2018

  Flush TLB on update displayed term (reset terminal paging): This was a bug resulting from forgetting to flush the TLB when chaging the paging structure to reflect a change in the displayed terminal.The bug caused problems with video memory from one terminal bleeding into another terminal (seen when running pingpong and counter at the same time). The bug was fixed by simply adding a function call to flush the TLB when changing the terminal paging entries.
  Fixed By: Pat
  Fixed On: 12/9/2018

  Synchronization issues in system calls and terminal code: We did not initially add synchronization to certain system calls (read, write, and vidmap) that either modified paging structure entries, copied data from kernel space to user space, or began or halted execution of a process (execute and halt). This led to seemingly random exception generation (debug exception, general protection fault, page fault) on both displayed and non-displayed terminals. It was fixed by checking which functions needed to have the interrupt flag cleared, then saving and restoring flags. Processes that needed to set the interrupt flag upon returning only used sti(), not restore flags.
  Fixed by: Pat, Ryan
  Fixed On: 12/9/2018
