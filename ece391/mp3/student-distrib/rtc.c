#include "i8259.h"
#include "lib.h"
#include "rtc.h"
#include "terminal.h"

/* rtc_init
 * Initialize RTC register values
 * args: none
 * returns: none
 */
void rtc_init(void){
	uint32_t flags;
	cli_and_save(flags);
	rtc_int = 0;					// no interrupt in progress
	rtc_count = 0;				// no interrupts have occurred yet

	outb(REGA, RTC_PORT);	// select Status Register A, and disable NMI (by setting the 0x80 bit)
	outb(FREQ, RTC_PORT+1);	// write to CMOS/RTC RAM (slow frequency to show keyboard interrupts)

	outb(REGB, RTC_PORT);		// select register B, and disable NMI
	char prev = inb(RTC_PORT+1);	// read the current value of register B
	outb(REGB, RTC_PORT);		// set the index again (a read will reset the index to register D)
	outb(prev | 0xF0, RTC_PORT+1);	// write the previous value ORed with 0x40. This turns on bit 6 of register B

	enable_irq(RTC_IRQ_NUM);
	enable_irq(MASTER_SLAVE_IRQNUM);

	restore_flags(flags);		// (perform an STI) and reenable NMI if you wish
}

/* rtc_handler
 * Description: handles RTC interrupts
 * Input: none
 * Output: none
 * Return: none
 * Side effecs: calls the given test_interrupts() functions to visually test RTC interrupts
 */
void rtc_handler(void) {
	disable_irq(RTC_IRQ_NUM);
	send_eoi(RTC_IRQ_NUM);

	rtc_int = 1; 		//indicates interrupt in progress
	enable_irq(RTC_IRQ_NUM);
	outb(READC, RTC_PORT);			//read the C register to ensure more than one interrupt will be read
	inb(RTC_PORT+1);					//we use the RTC essentially just a counter so the value in register C can be thrown out
}

/* rtc_write
 * DESCRIPTION: changes the frequency of RTC interrupts
 * INPUT: new desired frequency (power of 2) of interrupts
 * OUTPUT: returns 0 on successful frequency change, returns -1 on failure to change
 * SIDE EFFECT: changes RTC registers to reflect frequency change
 */
int rtc_write(uint32_t freq){
	uint8_t rate = 0;

	while(freq < MAX_FREQ){		// calculate the rate
		rate++;
		freq *= 2;
	}
	rate += 1;
	if(rate > MAX_RATE || rate < MIN_RATE){		// if the rate is out of bounds, return -1 on failure
		return -1;
	}
	unsigned int flags;
	cli_and_save(flags);

	outb(REGA, RTC_PORT);					// select register A
	char prev=inb(RTC_PORT + 1);	// get initial value of register A
	outb(REGA, RTC_PORT);					// reset index to A
	outb((prev & 0xF0) | rate, RTC_PORT + 1); //write only our rate to A. Note, rate is the bottom 4 bits.
	
	restore_flags(flags);
	return 0;				//return zero on success
}

/* rtc_read
 * DESCRIPTION: waits for an interrupt to occur, then returns to notify user level program
 * INPUT:	none
 * OUTPUT: returns 0 once interrupt has occurred
 * SIDE EFFECT: none
 */
int rtc_read(void){
	rtc_int = 0;
	while(!rtc_int);
	return 0;
}

/* rtc_open
 * DESCRIPTION: initializes the RTC interrupt frequency to 2 Hz
 * INPUT: none
 * OUTPUT: returns 0
 * SIDE EFFECT: changes RTC registers to reflect frequency change
 */
int rtc_open(void){				//rtc init sets the frequency to the default frequency (2 Hz) as well
	unsigned int flags;
	cli_and_save(flags);

	uint8_t defaultrate = FREQ;
	outb(REGA, RTC_PORT);					// select register A
	char prev=inb(RTC_PORT + 1);	// get initial value of register A
	outb(REGA, RTC_PORT);					// reset index to A
	outb((prev & 0xF0) | defaultrate, RTC_PORT + 1); //write only our rate to A. Note, rate is the bottom 4 bits.

	restore_flags(flags);
	return 0;
}

/* rtc_close
 * DESCRIPTION: does nothing (no cleanup work to do)
 * INPUT: none
 * OUTPUT: returns 0
 * SIDE EFFECT: none
 */
int rtc_close(void){
	return 0;
}
