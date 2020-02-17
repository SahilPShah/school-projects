/*
 * tuxctl-ioctl.c
 *
 * Driver (skeleton) for the mp2 tuxcontrollers for ECE391 at UIUC.
 *
 * Mark Murphy 2006
 * Andrew Ofisher 2007
 * Steve Lumetta 12-13 Sep 2009
 * Puskar Naha 2013
 */

#include <asm/current.h>
#include <asm/uaccess.h>

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/sched.h>
#include <linux/file.h>
#include <linux/miscdevice.h>
#include <linux/kdev_t.h>
#include <linux/tty.h>
#include <linux/spinlock.h>

#include "tuxctl-ld.h"
#include "tuxctl-ioctl.h"
#include "mtcp.h"

/*private helper functions*/
void init_tux(struct tty_struct* tty);
int tux_set_led(struct tty_struct* tty, unsigned long arg);
int tux_buttons(struct tty_struct* tty, unsigned long arg);

/*Global vars*/
unsigned long button_val = 0;
volatile unsigned long ledval = 0;
volatile int tux_ack = 0;            //tux is no longer being written to (i.e the tux has acknoledged the previous action)
volatile unsigned char letter_buttons = 1, arrow_keys = 1;

/*spin locks to be used*/
static spinlock_t b_lock = SPIN_LOCK_UNLOCKED;
static spinlock_t packet_lock = SPIN_LOCK_UNLOCKED;
static spinlock_t rst_lock = SPIN_LOCK_UNLOCKED;


#define debug(str, ...) printk(KERN_DEBUG "%s: " str, __FUNCTION__, ## __VA_ARGS__)

unsigned char led_map[16] = {0xE7, 0x06, 0xCB, 0x8F,
                                0x2E, 0xAD, 0xED, 0x86,
                                0xEF, 0xAF, 0xEE, 0x6D,
                                0xE1, 0x4F, 0xE9, 0xE8};

/************************ Protocol Implementation *************************/

/* tuxctl_handle_packet()
 * IMPORTANT : Read the header for tuxctl_ldisc_data_callback() in
 * tuxctl-ld.c. It calls this function, so all warnings there apply
 * here as well.
 *DESCRIPTION: this function handles packets sent from the tux controller
 *   INPUTS: tty_struct* tty, struct file* fd, unsigned char* packet
 *   OUTPUTS: none
 *   RETURN VALUE: 0 if checks pass, -1 if any fail
 *   SIDE EFFECTS: saves the packets to be accessed by the buttons handler
 */
void tuxctl_handle_packet (struct tty_struct* tty, unsigned char* packet) {
    unsigned a, b, c;
    unsigned int flags;
    a = packet[0]; /* Avoid printk() sign extending the 8-bit */
    b = packet[1]; /* values when printing them. */
    c = packet[2];
    switch(a){
      case MTCP_ACK:
        tux_ack = 0;
        break;
      case MTCP_BIOC_EVENT:
        spin_lock_irqsave(&packet_lock,flags);
        letter_buttons = b;
        arrow_keys = c;
        spin_unlock_irqrestore(&packet_lock,flags);
        break;
      case MTCP_RESET:
        spin_lock_irqsave(&rst_lock,flags);
        tux_ack = 0;
        init_tux(tty);
        tux_set_led(tty, ledval);
        spin_unlock_irqrestore(&rst_lock,flags);
        break;

    }

    /*printk("packet : %x %x %x\n", a, b, c); */
}

/******** IMPORTANT NOTE: READ THIS BEFORE IMPLEMENTING THE IOCTLS ************
 *                                                                            *
 * The ioctls should not spend any time waiting for responses to the commands *
 * they send to the controller. The data is sent over the serial line at      *
 * 9600 BAUD. At this rate, a byte takes approximately 1 millisecond to       *
 * transmit; this means that there will be about 9 milliseconds between       *
 * the time you request that the low-level serial driver send the             *
 * 6-byte SET_LEDS packet and the time the 3-byte ACK packet finishes         *
 * arriving. This is far too long a time for a system call to take. The       *
 * ioctls should return immediately with success if their parameters are      *
 * valid.                                                                     *
 *                                                                            *
 ******************************************************************************/
 /*
  * tuxctl_ioctl
  *   DESCRIPTION: this function acts as a "dispatcher" to each IOCTL function
  *   INPUTS: tty_struct* tty, struct file* fd, unsigned cmd, unsigned long arg
  *   OUTPUTS: none
  *   RETURN VALUE: 0 if checks pass, -1 if any fail
  *   SIDE EFFECTS: redirects to the specific ioctl based on the cmd arguement
  */
int tuxctl_ioctl(struct tty_struct* tty, struct file* file,
                 unsigned cmd, unsigned long arg) {
    unsigned int flags;
    int ret;
    switch (cmd) {
        case TUX_INIT:
            init_tux(tty);
            break;
        case TUX_BUTTONS:
            spin_lock_irqsave(&b_lock,flags);
            ret = tux_buttons(tty,arg);
            spin_unlock_irqrestore(&b_lock, flags);
            return ret;
            break;
        case TUX_SET_LED:
            ret = tux_set_led(tty, arg);
            return ret;
            break;
        default:
            return -EINVAL;
    }
    return 0;
}
/*
 * init_tux
 *   DESCRIPTION: this function initializes the tux with the BIOC_EVENT and LED_USR signals which enable button interrupts
 *                and allow the user to set the led values
 *   INPUTS: tty_struct* tty
 *   OUTPUTS: none
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
void init_tux(struct tty_struct* tty){
    char init_args[2] = {MTCP_BIOC_ON, MTCP_LED_USR};
    tuxctl_ldisc_put(tty, init_args, 2);
    return;
}
/*
 * set_led
 *   DESCRIPTION: this function sets the led values on the tux controllers
                  the value displayed is represented by the low 2 bytes of arg,
                  the led enable bits are the low 4 bits of the 3rd byte, and
                  the decimal point enable bits are the low 4 bits of the 4th byte
 *   INPUTS: tty_struct* tty, unsigned long arg
 *   OUTPUTS: sets the value of the leds
 *   RETURN VALUE: none
 *   SIDE EFFECTS: none
 */
int tux_set_led(struct tty_struct* tty, unsigned long arg){
    int hex_val,led_sel, dp_enable, digit, bitmap;
    unsigned char led_buf[LED_BUF_SIZE];
    int i;
    if(tux_ack){
      return 0;
    }
    tux_ack = 1;
    ledval = arg;                       //store the arguement
    hex_val = arg & LOW16_MASK;            //get the hex value for to be printed to the LEDs
    led_sel = (arg & LED_SELECT_MASK) >> LED_SHIFT;         //get LED select value
    dp_enable = (arg & DP_ENABLE) >> DP_SHIFT;     //get the decimal point enable bit..puts decimal enable bits into LSB

    led_buf[0] = MTCP_LED_SET;
    led_buf[1] = ENABLE_LED;        //enable all LEDS
    /*
    *The rest of the LED buffer is as follows
    *led_buf[2] = LED0, led_buf[3] = LED1, etc
    */
    for(i = 2; i < LED_BUF_SIZE; i++){
        led_buf[i] = 0x0;
        if((led_sel & 0x01)){         //if the LED is on
            //get the current digit and put its bit mapping into the buffer
            digit = hex_val & DIGIT;           //get the current digit to be printed
            bitmap = led_map[digit];
            if(dp_enable & 0x01)
                bitmap += DECIMAL_ON;
            led_buf[i] = bitmap;
        }

        led_sel = led_sel >> 1;
        hex_val = hex_val >> DIGIT_SHIFT;
        dp_enable = dp_enable >> 1;
    }


    tuxctl_ldisc_put(tty, led_buf, LED_BUF_SIZE);
    return 0;
}
/*
 * tux_buttons
 *   DESCRIPTION: this function takes the button data from the handler, parses it, and copies it
*                 to user space
 *   INPUTS: tty_struct* tty, unsigned long arg
 *   OUTPUTS: none
 *   RETURN VALUE: return 0 on finish
 *   SIDE EFFECTS: none
 */
int tux_buttons(struct tty_struct* tty, unsigned long arg){
  unsigned int letters = letter_buttons & DIGIT_MASK;
  unsigned int arrows = arrow_keys & DIGIT_MASK;
  unsigned long *ptr_to_usr;

  unsigned int down = (arrows & DOWN) << 1; //move the down signal left one bit
  unsigned int left = (arrows & LEFT) >> 1; //move the left signal right one bit
  unsigned int low2 = (arrows & RIGHTUP);      //get the bottom two bits

  arrows = down | left | low2;              //recombine them

  arrows = arrows << DIGIT_SHIFT;
  button_val = arrows | letters;
  ptr_to_usr = &button_val;
  if(!arg)
    return -EINVAL;
  copy_to_user((int*)arg, ptr_to_usr, sizeof(int*));
  return 0;
}
