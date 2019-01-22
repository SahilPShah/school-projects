#include "terminal.h"

char* static_video_mem = (char* )VIDEO; 	// points to where video memory is, do not change this!

extern char* video_mem;	            // video memory pointer to where to draw to, change this when context switching
									// NOT the same as what is showed on screen

// used to keep track of keystrokes and to send commands
char buf[BUF_SIZE];
uint8_t buf_count;

/* clear_terminal
 * Description: erases all visible characters from the screen btween upper and lower borders
 								almost the same as clear in lib.c, but has the set_screen on the bottom and doesn't clear upper/lower border
 * Input: none
 * Output: none
 * Return: 0 on success
 * Special Effects: changes the appearance of the screen by erasing all visible characters and moving cursor
 */
int clear_terminal(void){
	uint32_t flags;
	cli_and_save(flags);
	uint8_t saved_term = current_term;
	if(saved_term != displayed_term)
		update_current_term(displayed_term);

	int32_t i;
    for (i = NUM_COLS*UPPER_BORDER; i < NUM_COLS * (NUM_ROWS-LOWER_BORDER); i++) {
        *(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
    }
	set_screen(0, UPPER_BORDER);
	update_cursor();	//clear terminal should not clear the buffer!
	if(saved_term != current_term)
		update_current_term(saved_term);

	//sti();
	restore_flags(flags);
	return 0;
}


/* Writes a character to video memory.
- Should be called in keyboard.c
- Accounts for special cases
	- CTRL + L = clear, handled in keyboard driver
	- text wraparound
	- backspace
	- Enter
- Keeps track of its value in the buffer
- Note: prev_x needs to be signed integer

Input : char c, the character to be written
Output: the character is written to the terminal
Return: none
Special effects: checks and handles for special scancodes: ENTER and BACKSPACE
*/
void write_character(char c){
	int prev_x;
	int prev_y;

	switch(c){
		case(BACKSPACE): // Backspace
			if(buf_count == 0){
				break;}
			if(get_y() == UPPER_BORDER && !get_x())
				break;
			//Calculate what the previous character was (to unwrite-it and then set the cursor there)
			prev_y = get_y();
			prev_x = get_x()-1;
			if(prev_x < 0){ // If typing was on a new line (first character)
				prev_x = NUM_COLS-1;
				prev_y--;
			}
			if (prev_y < 0 || prev_x < 0){printf("ERROR: prevx/prevy values\n");}

			//Clear the previous character and update the cursor
			target_write_character(' ', prev_x, prev_y);
			set_screen(prev_x, prev_y);
			buf_count--;
			buf[buf_count] = 0;
			break;

		case(ENTER): // Enter
			if(get_y() == NUM_ROWS-LOWER_BORDER-1){
				scroll_screen(0);
			}
			printf("%c", ENTER);
			break;

		default: // regular ascii char
			printf("%c", c);

	}
	if(current_term == displayed_term)
		update_cursor();
}

/* Sets the foreground and background colors of a place in video memory. refer to https://wiki.osdev.org/Text_UI
Bit 76543210
    ||||||||
    |||||^^^-fore colour
    ||||^----fore colour bright bit
    |^^^-----back colour
    ^--------back colour bright bit OR enables blinking Text
Does not update the cursor, oes not reset screen_x and screen_y
Input: char c, dest_x, dest_y. The byte to be written, the destination coordinates in the terminal
Ouput: rights the character into video memory
Return: none
Special Effect: none
*/
void write_format(char c, uint32_t dest_x, uint32_t dest_y){
	*(uint8_t *)(video_mem + ((NUM_COLS * dest_y + dest_x) << 1) + 1) = c;
}

/* Writes a specific character to the dest_x, dest_y value
notes: does NOT reset screen_x and screen_y, does NOT update the cursor, does NOT add to the buffer.
Input: char c, dest_x, dest_y. The byte to be written, the destination coordinates in the terminal
Ouput: rights the character into video memory
Return: none
Special Effect: none
*/
void target_write_character(char c, uint32_t dest_x, uint32_t dest_y){
	*(uint8_t *)(video_mem + ((NUM_COLS * dest_y + dest_x) << 1)) = c;
}

/* get_border
 * Description: gets the border values (0: upper border, 1: lower border)
 * Input: uint8_t c: byte specifier for the specifc border to be returned
 * Output: none
 * Return: LOWER or UPPER BORDER
 * Special Effects: none
 */
uint8_t get_border(uint8_t c){
	if(c)
		return LOWER_BORDER;
	return UPPER_BORDER;
}

/* scrolls the screen up. Later update this to use a include real scrolling
args: direction
	0: up
	1: down (not implemented)
todo (eventually): implement real vertical scrolling (big video buffer)
Input: uint8_t direction: direction to scroll
Output: Moves data in video memory to scroll the screen
Return: none
Special Effects: none
*/
void scroll_screen(uint8_t direction){
	// Index of character on screen
	uint32_t i;

	// Scroll up, set value of row to the row below it (except for borders, and for last row before the bottom border)
	for (i = NUM_COLS*UPPER_BORDER; i < NUM_COLS * (NUM_ROWS-LOWER_BORDER-1); i++) {
        *(uint8_t *)(video_mem + (i << 1)) = *(uint8_t *)(video_mem + ((i+NUM_COLS) << 1));
        *(uint8_t *)(video_mem + (i << 1) + 1) = *(uint8_t *)(video_mem + ((i+NUM_COLS) << 1) + 1);
    }

    for(i = NUM_COLS * (NUM_ROWS-LOWER_BORDER-1); i < NUM_COLS * (NUM_ROWS-LOWER_BORDER); i++){
    	*(uint8_t *)(video_mem + (i << 1)) = ' ';
        *(uint8_t *)(video_mem + (i << 1) + 1) = ATTRIB;
    }
    //Update cursor location
    if (get_y() == 0){
    	set_screen(get_x(), 0);
    }
    else{
    	set_screen(get_x(), get_y()-1);
    }
		if(current_term == displayed_term)
    	update_cursor();
}

/* draw_borders
 * Description: draws borders. background color set to blue, text white
 * Input: none
 * Output: none
 * Return: none
 * Special Effects: modifies screen by changing color of sections of the screen
 */
void draw_borders(){/*
	uint32_t i, j;
	for(i = 0; i < UPPER_BORDER; i++){						// for all screen locations above upper border, change the color
		for (j = 0; j < NUM_COLS; j++){
			write_format(BORDER_ATTRIB, j, i);
			target_write_character(' ', j, i);
		}
	}
	for(i = 0; i < LOWER_BORDER; i++){						// for all screen locations below lower border, change the color
		for (j = 0; j < NUM_COLS; j++){
			write_format(BORDER_ATTRIB, j, NUM_ROWS-1-i);
			target_write_character(' ', j, NUM_ROWS-1-i);
		}
	}
	target_write_character('S', 36, 1);						// print some header text above the upper border (character, x-coord , y-coord)
	target_write_character('P', 37, 1);
	target_write_character('a', 38, 1);
	target_write_character('R', 39, 1);
	target_write_character('K', 40, 1);
	target_write_character('o', 41, 1);
	target_write_character('s', 42, 1);*/
}

/* write_to_buffer
 * Description: stores a character from keyboard interrupt handler in the buffer
 * Input: new character to store in the buffer
 * Output: none
 * Return: none
 * Special Effects: modifies keystroke buffer and calls function to print character to the screen
 */
void write_to_buffer(uint8_t new_char) {
	int flags;
	cli_and_save(flags);
	uint8_t saved_term = current_term;

	// switch to displayed terminal
	update_current_term(displayed_term);
	if(buf_count >= BUF_SIZE-1 && new_char != ENTER && new_char != BACKSPACE)
		goto switch_back;
	
    // don't store backspace character in buffer, just print it
	if(new_char == BACKSPACE){			
		write_character(new_char);
		goto switch_back;
	}
	else if(new_char == ENTER) {
		terminal_data[displayed_term].read_waiting = 0;
		buf[buf_count] = new_char;
		buf_count++;
		if(buf_count != BUF_SIZE)
			buf[buf_count] = 0;
		write_character(new_char);
		goto switch_back;
	}

	// add character to buffer and print normally if not backspace or enter
	buf[buf_count] = new_char;			
	buf_count++;
	write_character(new_char);

	// switch back
	switch_back:
		update_current_term(saved_term);
		restore_flags(flags);
}

/* clear_buffer
 * Description: clears the buffer on enter key press
 * Input: none
 * Output: none
 * Return: returns 0 on success
 * Special Effects: clears keypress buffer and resets number of characters stored in buffer
 */
int clear_buffer() {
	int i;
	buf_count = 0;
	for(i = 0; i < BUF_SIZE; i++)
		buf[i] = 0;
	return 0;
}

/* buf_size
 * Description: reads current number of characters in buffer
 * Input: none
 * Output: none
 * Return: returns number of characters in the keystroke buffer
 * Special Effects: none
 */
int buf_size(){
	return buf_count;
}

/* update_displayed_term(uint32_t scan_code)v
Switches terminals, saves some data and stores some data

Description:
- moves paging info for current term back to the current term destination
- moves current video mem data into storage (previous terminal)
- moves curser and buf into storage (previous terminal)
- copy target terminal memory into video mem
- update cursor and buf data into current

buffer tied to displayed/target,
cursor is tied to current (buffer and text)
update_cursor is tied to displayed

keyboard interrupts will context switch to the disaplyed terminal for the text, then switch back to current
(updates buffer and cursors)

Displayed/Current/Target Alg:
Displayed   Current   	Target  update         Screen     Displayed Buffer Actual Buffer Displayed cursor  Actual Cursor
x			x			x		Do Nothing
x 			x  			y                      yes        yes              doesnt matter yes               No
x 			y 			x 		Do Nothing
x 			y 			y  		Update screen
x  			y 			z		update screen, buffer, position	-- untested
*Inputs: scan_code (from keyboard.c)
*Outputs: none
*Return: 0 for success
*SFX: changes the terminal that is displayed: This requires saving the current terminal video memory, changing the paging structure for VM,
*updating the current cursor, and setting the screen
*/
int update_displayed_term(uint32_t scan_code){
	//todo: cli and sti aroudn entire thing?
	int flags;
	cli_and_save(flags);
	// interpret scan code to get the target terminal (either 0, 1, or 2 for terminal 1, 2, or 3)
	uint8_t target_term;// int, either 0, 1, 2
	switch(scan_code){
		case ALT_F1:
			target_term = 0;
			break;
		case ALT_F2:
			target_term = 1;
			break;
		case ALT_F3:
			target_term = 2;			
			break;
		default:
			printf("default switch terminal invalid scan_code error");
	}
	if(target_term == displayed_term){
		//sti();
		restore_flags(flags);
		return 0;
	}

	// reset paging of displayed_term to point to default location, instead of video memory
	reset_terminal_paging(displayed_term, 1);

	// move data from current terminal into storage
	memcpy(get_terminal_address(displayed_term), static_video_mem, NUM_ROWS*NUM_COLS*2);

	// move data from target terminal into video memory
	memcpy(static_video_mem, get_terminal_address(target_term), NUM_ROWS*NUM_COLS*2);

	if(VMpagetable_u[VMPTIDX_T0 + displayed_term] != 0)
		VMpagetable_u[VMPTIDX_T0 + displayed_term] = (uint32_t)(get_terminal_address(displayed_term)) + USER + RW + PRESENT;
	if(VMpagetable_u[VMPTIDX_T0 + target_term] != 0)
		VMpagetable_u[VMPTIDX_T0 + target_term] = VMADDR + USER + RW + PRESENT;

	// update the cursor, but don't actually update screenx and screeny
	int current_x = get_x();
	int current_y = get_y();
	if(target_term != current_term){
		// temporarily set the screen to the target_term's cursor location (for update_cursor())
		set_screen(terminal_data[target_term].screen_x, terminal_data[target_term].screen_y);
	}
	update_cursor();

	// change the writing location back to the current_term coords
	set_screen(current_x, current_y);

	// update paging so target terminal now writes to video mem
	reset_terminal_paging(target_term, 0);

	displayed_term = target_term;
	restore_flags(flags);
	return 1;
}

/*
get_terminal_address(uint8_t terminal_num)
Description: helper function to get vaddr of a terminal
Inputs: terminal_num
Outputs: none
Returns: virtual address for the terminal
SFX: none
notes: uses defined values in paging.h
*/
char* get_terminal_address(uint8_t terminal_num){
	if(terminal_num >= NUM_TERMS){ // terminal_num is 0 indexed
		return NULL;
	}
	void* ret = (void*)(TERM1_VADDR + terminal_num*TERM_FENCE);
	return (char*)ret; // must reflect data in paging.h
}

/*
int update_current_term(uint8_t target_term)
Description: Used to write to a terminal that is not active. **Use this during scheduling**
Inputs: target_term: terminal to now write text to
Outputs: none
Return: the number of changes made (0 or 1)
SFX: none
Notes: Changes where the keyboard and where printf writes to, doesn't care about displayed_term
still needs to be tested thoroughly
*/
int update_current_term(uint8_t target_term){
	if (current_term == target_term){
		return 0;
	}
	int flags;
	cli_and_save(flags);

	// save terminal data of current terminal
	terminal_data[current_term].screen_x = get_x();
	terminal_data[current_term].screen_y = get_y();
	terminal_data[current_term].buf_count = buf_count;
	memcpy(terminal_data[current_term].buf, buf, BUF_SIZE);

	// load data of target terminal
	set_screen(terminal_data[target_term].screen_x, terminal_data[target_term].screen_y);
	memcpy(buf, terminal_data[target_term].buf, BUF_SIZE);
	buf_count = terminal_data[target_term].buf_count;

	// update global values
	current_term = target_term;
	video_mem = get_terminal_address(target_term);

	restore_flags(flags);
	return 1;
}

// reads the buffer (probably need to change return type and arguments, return char*, take num bytes/chars to read?)
/*
*Input: char* to_read, int32_t nbytes: the character buffer to put the read data into and the number of total bytes to read
		ignores are used to interface with system calls (to match the one for file_read
Output:  the character buffer is filled
Return: 0 for success, -1 for fail
SFX: none
*/
int terminal_read(uint32_t ignore, char* to_read, int32_t nbytes, uint32_t ignore2) {
	terminal_data[current_term].read_waiting = 1;
	while(terminal_data[current_term].read_waiting || (terminal_data[current_term].active_pid != terminal_data[displayed_term].active_pid));

	if(!to_read) return -1;
	uint32_t flags;
	cli_and_save(flags);

	uint32_t index = 0;
	while(index < nbytes && index < buf_count){
		to_read[index] = buf[index];
		index++;
	}
	clear_buffer();
	memcpy(terminal_data[current_term].buf, buf, BUF_SIZE);
	terminal_data[current_term].buf_count = buf_count;

	restore_flags(flags);
	return index;
}

/*
Description: writes a string stored in a buffer to screen
Input: char* to_read, int32_t nbytes: number of bytes to write to the terminal from the input buffer
Output:  the character buffer is written to the string
Return: 0 for success, -1 for fail
SFX: none
*/
int terminal_write(const void* to_write, int32_t nbytes) {
	int flags;
	if(!to_write) return -1;
	cli_and_save(flags);
	int index = 0;

	while (index < nbytes) {			
			write_character(((uint8_t*)to_write)[index]);
			index++;
	}

	restore_flags(flags);
	return nbytes;
}

/*
desc: initializes the terminal
Input: none
Output:  none
Return: 0 for success, -1 for fail
SFX: none
*/
int terminal_open() {
	clear_buffer();
	clear_terminal();
	int x;

	// initialize terminal data structs to have no info
	for (x = 0; x < NUM_TERMS; x++){
		terminal_data[x].screen_x = 0;
		terminal_data[x].screen_y = 0;
		terminal_data[x].read_waiting = 1;
		terminal_data[x].rtc_freq = 0;
		int y;
		for (y = 0; y < BUF_SIZE; y++){
			terminal_data[x].buf[y] = 0;
		}
	}

	// initial terminals to have spaces (0x20 and 0x07)
	for ( x = 0; x < NUM_ROWS*NUM_COLS; x++){
		*(char*)(TERM1_VADDR+x*2) = SPACE;
		*(char*)(TERM1_VADDR+x*2+1) = VM_FILLER_BYTE;
		*(char*)(TERM2_VADDR+x*2) = SPACE;
		*(char*)(TERM2_VADDR+x*2+1) = VM_FILLER_BYTE;
		*(char*)(TERM3_VADDR+x*2) = SPACE;
		*(char*)(TERM3_VADDR+x*2+1) = VM_FILLER_BYTE;
	}

	//setup so terminal 1 is displayed
	reset_terminal_paging(0, 0); // set terminal 1 to point to vmem
    video_mem = get_terminal_address(0);
    displayed_term = 0;
    current_term = 0;

	return 0;
}


/*
desc: closes the terminal, does cleanup work
Input: none
Output:  none
Return: 0 for success, -1 for fail
SFX: none
*/
int terminal_close() {
	return 0;
}
