#ifndef _TERMINAL_H
#define _TERMINAL_H

#include "x86_desc.h"
#include "lib.h"
#include "paging.h"

#define SPACE       0x20
#define VM_FILLER_BYTE 0x07
#define VIDEO       0xB8000
#define NUM_COLS    80
#define NUM_ROWS    25
#define BUF_SIZE		128
#define ATTRIB        0x7
#define BORDER_ATTRIB 0x3F    // First number is the color of the background, second is the color of the text

#define ENTER			0x0a        // ASCII character codes
#define BACKSPACE 0x08

// Values found by looking in ece391hello.c. Are these correct/needed?
#define TERMINAL_WRITE_FD 1
#define TERMINAL_READ_FD  0

// Frozen rows in the terminal. Can be used for fancy UI stuff later. draw_borders must be called to write to these addresses, otherwise they won't be used. i think.
#define UPPER_BORDER	0
#define LOWER_BORDER	0

// stuff for switching terminals.
#define ALT_F1	  0x3B
#define ALT_F2    0x3C
#define ALT_F3    0x3D

#define NUM_TERMS 			3
#define VMPTIDX_T0    256

typedef struct term_data{
	uint32_t screen_x;
	uint32_t screen_y;
	uint32_t esp;
	uint32_t ebp;
	uint32_t ss0;
	uint32_t esp0;
	uint32_t rtc_freq;
	uint8_t active_pid; 					//the active pid running on this terminal
	uint8_t buf_count;
	uint8_t buf[BUF_SIZE];
	volatile uint8_t read_waiting;
}term_data_t;

term_data_t terminal_data[NUM_TERMS];
uint8_t current_term;				// the terminal that is currently being written to, is tied with video_mem, buf, and screen x/y
// use to keep track of terminal data for switching
uint8_t displayed_term;			    // The terminal that is DISPLAYED, 0, 1, or 2. does not affect buf, screenx/y, just whats on screen
uint8_t terms_init;
// use set_screen to change writing cursor (written in lib.c)
// use update_cursor to change blinking cursor (written in lib.c)

/* clears the terminal. Resets the cursor location and clears text in the terminal */
int clear_terminal(void);

// Writes a character at the spot. Doesn't affect the buffer/etc, can be used for debugging
void target_write_character(char c, uint32_t dest_x, uint32_t dest_y);

// generic character write.
void write_character(char c);

// Formats a location. read the .c for more details
void write_format(char c, uint32_t dest_x, uint32_t dest_y);

// Helper function to get border boundaries (to interface with printf in lib.c)
uint8_t get_border(uint8_t dir);

// scrolls the screen
void scroll_screen(uint8_t direction);

// draws borders
void draw_borders();

// stores a character from keyboard interrupt handler in the buffer
void write_to_buffer(uint8_t new_char);

// clears the buffer on enter key press or ctrl-L
int clear_buffer();

// reads size of keystroke buffer
int buf_size();

// reads the buffer
int terminal_read(uint32_t ignore, char* to_read, int32_t nbytes, uint32_t ignore2);

// writes a string to screen
int terminal_write(const void* to_write, int32_t nbytes);

// initializes the terminal by clearing buffer, screen
int terminal_open();

// closes the terminal, does cleanup work
int terminal_close();

// Switches between terminal 1, 2, 3 (scan does are x68, x69, and x6A
int update_displayed_term(uint32_t scan_code);

char* get_terminal_address(uint8_t terminal_num);

int update_current_term(uint8_t target_term);

#endif
