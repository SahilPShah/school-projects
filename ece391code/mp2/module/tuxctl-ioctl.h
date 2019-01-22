#ifndef TUXCTL_H
#define TUXCTL_H

#define TUX_SET_LED _IOR('E', 0x10, unsigned long)
#define TUX_READ_LED _IOW('E', 0x11, unsigned long*)
#define TUX_BUTTONS _IOW('E', 0x12, unsigned long*)
#define TUX_INIT _IO('E', 0x13)
#define TUX_LED_REQUEST _IO('E', 0x14)
#define TUX_LED_ACK _IO('E', 0x15)


#define LED_BUF_SIZE        6
#define LOW16_MASK          0x0000FFFF
#define LED_SELECT_MASK     0x000F0000
#define DP_ENABLE           0x0F000000
#define DIGIT               0x0000000F
#define DECIMAL_ON          0x10            //00010000
#define LED_SHIFT           16
#define DP_SHIFT            24
#define ENABLE_LED          0xF
#define DIGIT_SHIFT         4
#define DIGIT_MASK          0x00F
#define DOWN                0x02
#define LEFT                0x04
#define RIGHTUP             0x09
#endif
