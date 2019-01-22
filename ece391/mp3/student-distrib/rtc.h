#ifndef _RTC_H
#define _RTC_H

#define RTC_IRQ_NUM	8           // IRQ priority number on PIC for RTC
#define MASTER_SLAVE_IRQNUM 2   // IRQ line on master that slave PIC is connected to
#define RTC_PORT	0x70          // RTC port number
#define REGA    0x8A            // Byte used to access reg A
#define FREQ    0x0F            // Default frequency for the RTC upon opening
#define REGB    0x8B            // Byte used to access reg B
#define READC   0x0C            // Byte used to access reg C
#define MAX_FREQ 32768
#define MIN_RATE 6
#define MAX_RATE 15


/* keeps track of whether interrupt is currently happening and number of interrupts that have happened*/
volatile int rtc_int;
volatile unsigned long rtc_count;

/* RTC interrupt control functions*/
void rtc_init(void);
void rtc_handler(void);
int rtc_open(void);
int rtc_close(void);
int rtc_read(void);
int rtc_write(uint32_t freq);

#endif
