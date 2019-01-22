#include "keyboard.h"
#include "i8259.h"
#include "lib.h"
#include "terminal.h"

#define KBD_IRQ_NUM  1
#define NULL_CHAR  0x00
#define LSHIFT    0x2a
#define LSHIFT_R  0xaa
#define RSHIFT    0x36
#define RSHIFT_R  0xb6
#define CTRL      0x1d
#define CTRL_R    0x9d
#define CAPS      0x3a
#define CLEAR_KEY 0x26

#define ALT       0x38
#define ALT_R     0xB8
#define F1	      0x3B
#define F2        0x3C
#define F3        0x3D


uint8_t ps2_to_ascii_upl_ups[99] = {    // uppercase letters and symbols (shift, no caps)
  NULL_CHAR, NULL_CHAR, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a,
  0x28, 0x29, 0x5f, 0x2b, 0x08, NULL_CHAR, 0x51, 0x57, 0x45, 0x52,
  0x54, 0x59, 0x55, 0x49, 0x4f, 0x50, 0x7b, 0x7d, 0x0a, NULL_CHAR,
  0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x3a,
  0x22, 0x7e, NULL_CHAR, 0x7c, 0x5a, 0x58, 0x43, 0x56, 0x42, 0x4e,
  0x4d, 0x3c, 0x3e, 0x3f, NULL_CHAR, NULL_CHAR, NULL_CHAR, 0x20, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR
};

uint8_t ps2_to_ascii_lol_ups[99] = {    // lowercase letters and uppercase symbols (shift, caps)
  NULL_CHAR, NULL_CHAR, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5e, 0x26, 0x2a,
  0x28, 0x29, 0x5f, 0x2b, 0x08, NULL_CHAR, 0x71, 0x77, 0x65, 0x72,
  0x74, 0x79, 0x75, 0x69, 0x6f, 0x70, 0x7b, 0x7d, 0x0a, NULL_CHAR,
  0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x3a,
  0x22, 0x7e, NULL_CHAR, 0x5c, 0x7a, 0x78, 0x63, 0x76, 0x62, 0x6e,
  0x6d, 0x3c, 0x3e, 0x3f, NULL_CHAR, NULL_CHAR, NULL_CHAR, 0x20, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR
};

uint8_t ps2_to_ascii_upl_los[99] = {    // uppercase letters and lowercase symbols (no shift, caps)
  NULL_CHAR, NULL_CHAR, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
  0x39, 0x30, 0x2d, 0x3d, 0x08, NULL_CHAR, 0x51, 0x57, 0x45, 0x52,
  0x54, 0x59, 0x55, 0x49, 0x4f, 0x50, 0x5b, 0x5d, 0x0a, NULL_CHAR,
  0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4a, 0x4b, 0x4c, 0x3b,
  0x27, 0x60, NULL_CHAR, 0x5c, 0x5a, 0x58, 0x43, 0x56, 0x42, 0x4e,
  0x4d, 0x2c, 0x2e, 0x2f, NULL_CHAR, NULL_CHAR, NULL_CHAR, 0x20, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR
};

uint8_t ps2_to_ascii_lol_los[99] = {    // lowercase letters and symbols (no shift, no caps)
  NULL_CHAR, NULL_CHAR, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
  0x39, 0x30, 0x2d, 0x3d, 0x08, NULL_CHAR, 0x71, 0x77, 0x65, 0x72,
  0x74, 0x79, 0x75, 0x69, 0x6f, 0x70, 0x5b, 0x5d, 0x0a, NULL_CHAR,
  0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6a, 0x6b, 0x6c, 0x3b,
  0x27, 0x60, NULL_CHAR, 0x5c, 0x7a, 0x78, 0x63, 0x76, 0x62, 0x6e,
  0x6d, 0x2c, 0x2e, 0x2f, NULL_CHAR, NULL_CHAR, NULL_CHAR, 0x20, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR,
  NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR
};

static uint8_t left_shift = 0;
static uint8_t right_shift = 0;
static uint8_t caps = 0;
static uint8_t ctrl = 0;
static uint8_t alt = 0;

/*
*Description: initializes the keyboard
*Input: none
*Output: none
*Return: none
*Side effecs: enables IRQ line 1 which is on the master pic
*/

void kbd_init(){
  enable_irq(KBD_IRQ_NUM);
}

/*
*Description: handles keyboard interrupts. Accept the scan code and
*translates it into the ASCII character to be printed
*Input: none
*Output: echoes character to the screen
*Return: none
*Side effects: disables IRQ line for the duration of the handler and re-enables it at the end
*              Sends EOI.
*/
void kbd_handler() {
  disable_irq(KBD_IRQ_NUM);
  send_eoi(KBD_IRQ_NUM);
  uint32_t scan_code = inb(KEYBOARD_INT);       //get the current scan code
  uint8_t ascii = 0;

  //determine if the following has been pressed
  if(scan_code == LSHIFT)
    left_shift = 1;
  if(scan_code == RSHIFT)
    right_shift = 1;
  if(scan_code == CTRL)
    ctrl = 1;
  if(scan_code == ALT)
    alt = 1;
  if(scan_code == CAPS) //Caps lock has no release because it is turned on/off with every press
    caps = ~caps;

  //determine if the following has been released
  if(scan_code == LSHIFT_R)
    left_shift = 0;
  if(scan_code == RSHIFT_R)
    right_shift = 0;
  if(scan_code == CTRL_R)
    ctrl = 0;
  if(scan_code == ALT_R)
    alt = 0;

  if(scan_code > 0x80)  //codes over x80 correspond to button releases. dont do anything in these cases
    goto endfn;

  if(alt && (scan_code == F1 || scan_code == F2 || scan_code == F3)){
	  update_displayed_term(scan_code);
  }
  else if(ctrl && scan_code == CLEAR_KEY){
    clear_terminal();
  }
  else if((left_shift || right_shift) && !caps) {
    ascii = ps2_to_ascii_upl_ups[scan_code];
    if(ascii == NULL_CHAR)
      goto endfn;
    else
      write_to_buffer(ascii);
  }
  else if((left_shift || right_shift) && caps) {
    ascii = ps2_to_ascii_lol_ups[scan_code];
    if(ascii == NULL_CHAR)
      goto endfn;
    else
      write_to_buffer(ascii);
  }
  else if(!(left_shift || right_shift) && caps) {
    ascii = ps2_to_ascii_upl_los[scan_code];
    if(ascii == NULL_CHAR)
      goto endfn;
    else
      write_to_buffer(ascii);
  }
  else {
    ascii = ps2_to_ascii_lol_los[scan_code];
    if(ascii == NULL_CHAR)
      goto endfn;
    else
      write_to_buffer(ascii);
  }

  endfn:
    enable_irq(KBD_IRQ_NUM);
}
