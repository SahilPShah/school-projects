#include "tests.h"
#include "x86_desc.h"
#include "lib.h"
#include "terminal.h"
#include "rtc.h"
#include "i8259.h"
#include "filesys.h"
#include "types.h"
#include "syscall.h"

#define PASS 1
#define FAIL 0

#define WAIT_2	2 // in rtc test 1
#define WAIT_4  4 // between tests
#define WAIT_6  6 // rtc test 2 and filesys

unsigned int __TEST_RTC__ = 0;			//test flag set to one when RTC testing enabled

/* format these macros as you see fit */
#define TEST_HEADER 	\
	printf("[TEST %s] Running %s at %s:%d\n", __FUNCTION__, __FUNCTION__, __FILE__, __LINE__)
#define TEST_OUTPUT(name, result)	\
	printf("[TEST %s] Result = %s\n", name, (result) ? "PASS" : "FAIL");

static inline void assertion_failure(){
	/* Use exception #15 for assertions, otherwise
	   reserved by Intel */
	asm volatile("int $15");
}

/* for fancy tests, waits for rtc to wait that long then celars terminal */
void wait_and_clear(int time){
	int interrupt_count = 0;
	while(interrupt_count <= time){
		rtc_read();		//rtc will return when an interrupt has occurred
		interrupt_count++;
	}
	clear_terminal();
}

/* to clear buff for read tests */
void clear_buf(char* buf, uint32_t size)
{
int i = 0;
	for (i = 0; i < size; i++){
		buf[i] = 0;
	}
}


/* Checkpoint 1 tests */

/* IDT Test - Example
 *
 * Asserts that first 10 IDT entries are not NULL
 * Inputs: None
 * Outputs: PASS/FAIL
 * Side Effects: None
 * Coverage: Load IDT, IDT definition
 * Files: x86_desc.h/S
 */
int idt_test(){
	TEST_HEADER;

	int i;
	int result = PASS;
	for (i = 0 ; i < 10; ++i){
		if ((idt[i].offset_15_00 == NULL) &&
			(idt[i].offset_31_16 == NULL)){
			assertion_failure();
			result = FAIL;
		}
	}

	return result;
}
/*
*Test case to test Divide by zero exception
*/
int test_div_by_zero(){
	int i = 1;
	int j = 1;
  int num = 1/(i-j);
	return num;
}

int test_paging(){
	int* ptr;
	char* ptr2;
	ptr = (int*)0x0b8000;
	printf("Attempting to dereference address 0x0B000 which is inside virtual memory.\nDo not expect a Page Fault\n");
	int val = *ptr;
	printf("val: %x\n", val);

	ptr = (int*)0x06B5FAE;
	printf("Attempting to dereference address 0x06B5FAE which is in kernel memory. \nDo not Expect a Page Fault\n");
	val = *ptr;
	printf("val: %x\n", val);

	ptr2 = (char*)0x07FFFFF;
	printf("Attempting to dereference address 0x07FFFFF which is end of kernel memory. \nDo not Expect a Page Fault\n");
	char val2 = *ptr2;
	printf("val: %x\n", val2);

	/*The next three test cases are expected to fail...uncomment to test each*/
	//test null
	ptr = (int*)0;
	printf("Attempting to dereference NULL.\nExpect a Page Fault\n");
	val = *ptr;
	// //test region between VM and kernel
	// ptr = (int*)0x03F0000;
	// printf("Attempting to dereference 0x03F0000 which is between VM and kernel.\nExpect a Page Fault\n");
	// val = *ptr;
	// //region of memory after kernel
	// ptr = (int*)0x0900000;
	// printf("Attempting to dereference 0x0900000 which is beyond kernelmemory .\nExpect a Page Fault\n");
	// val = *ptr;
	//
	return 1;
}

int test_keyboard(){
		printf("Please type the following: aAbBcC1234!@#$      ");
		return 1;
}

/* Checkpoint 2 tests */
int test_terminal(){
	clear_terminal();
	printf("Going to echo first 80 keyboard inputs to terminal upon ENTER press\n");
	int bytes_to_echo = 80;
	int bytes_typed;
	//while(buf_size() != bytes_to_echo);	//wait until the buffer has the number of characters to read/write
	char typed[bytes_to_echo];
	bytes_typed = terminal_read(0, typed, bytes_to_echo, 0);
	if (bytes_typed == -1){
		return 0;}
	if(terminal_write(typed, bytes_typed) == -1) return 0;

	return 1;
}

/*
*  This first RTC test tests all the possible frequencies that will fail
*	if any of these rtc_write's pass, the test case fails
*/
int test_rtc1(){
	clear_terminal();
	printf("Going to try to set frequencies greater than 1024 Hz\n");

	if(!rtc_write(32768))	//gives a rate of 1
		return 0;
	printf("Couldn't set 32768 Hz\n");
	wait(WAIT_2);

	if(!rtc_write(16384))	//gives a rate of 2
		return 0;
	printf("Couldn't set 16384 Hz\n");
	wait(WAIT_2);

	if(!rtc_write(8192))	//gives a rate of 3
		return 0;
	printf("Couldn't set 8192 Hz\n");
	wait(WAIT_2);

	if(!rtc_write(4096))	//gives a rate of 4
		return 0;
	printf("Couldn't set 4096 Hz\n");
	wait(WAIT_2);

	if(!rtc_write(2048))	//gives a rate of 5
		return 0;
	printf("Couldn't set 2048 Hz\n");
	wait(WAIT_2);

	return 1;
}

int test_rtc2(){
	clear_terminal();
	printf("Going to set RTC to different interrupt frequencies\n");
	wait(WAIT_6);

	printf("2 Hz\n");
	rtc_write(2);			//start at 2Hz
	int interrupt_count = 0;
	char flag = '1';
	while(interrupt_count <= 4){
		rtc_read();		//rtc will return when an interrupt has occurred
		interrupt_count++;
		write_character((uint8_t)flag);
	}
	clear_terminal();
	printf("16 Hz\n");
	rtc_write(16);			//16Hz
	interrupt_count = 0;
	flag = '1';
	while(interrupt_count <= 16){
		rtc_read();		//rtc will return when an interrupt has occurred
		interrupt_count++;
		write_character((uint8_t)flag);
	}
	clear_terminal();
	printf("32 Hz\n");
	interrupt_count = 0;
	if(rtc_write(32))	return 0;
	while(interrupt_count <= 32){
		rtc_read();
		interrupt_count++;
		write_character((uint8_t)flag);
	}
	clear_terminal();
	printf("512 Hz\n");
	interrupt_count = 0;
	if(rtc_write(512))	return 0;
	while(interrupt_count <= 512){
		rtc_read();
		interrupt_count++;
		write_character((uint8_t)flag);
	}
	clear_terminal();
	printf("1024 Hz\n");
	interrupt_count = 0;
	if(rtc_write(1024))	return 0;
	while(interrupt_count <= 1024){
		rtc_read();
		interrupt_count++;
		write_character((uint8_t)flag);
	}
	clear_terminal();
	rtc_write(2);
	return 1;
}

int test_filesys_contents_long()
{
	/*clear_terminal();
	printf("Going to read 1000 bytes from middle of verylargetextwithverylongname.txt\n");
	wait(WAIT_6);
	int size = 1000;
	int i;
	int8_t test_buf[size + 1];
	for(i = 0; i < size + 1; i++)
		test_buf[i] = 0;
	int32_t returnval = file_read("verylargetextwithverylongname.txt", test_buf, size, 4000);			// this needs to be updated to the right format
	if(returnval == -1) {
		printf("Invalid input\n");
		return 1;
	}
	terminal_write(test_buf, returnval);
	printf("\n");
	file_close();

	wait(WAIT_6);*/
	return 1;
}

int test_filesys_contents_text()
{
	/*
	clear_terminal();
	printf("Going to read all of frame0.txt\n");
	wait(WAIT_6);

	int size = 500;
	int i;
	int8_t test_buf[size + 1];
	for(i = 0; i < size + 1; i++)
		test_buf[i] = 0;
	int32_t returnval = file_read("frame0.txt", test_buf, size, 0);
	if(returnval == -1) {
		printf("Invalid input\n");
		return 1;
	}
	terminal_write(test_buf, returnval);
	printf("\n");
	file_close();

	wait(WAIT_6);
	*/
	return 1;
}

int test_filesys_contents_nontext()
{
	/*
	clear_terminal();
	printf("Going to read 1200 bytes from hello\n");
	wait(WAIT_6);

	int size = 1200;
	int i;
	int8_t test_buf[size + 1];
	for(i = 0; i < size + 1; i++)
		test_buf[i] = 0;
	int32_t returnval = file_read("hello", test_buf, size, 0);
	if(returnval == -1) {
		printf("Invalid input\n");
		return 1;
	}
	terminal_write(test_buf, returnval);
	printf("\n");
	file_close();

	wait(WAIT_6);
	*/
	return 1;
}

int test_filesys_dir_read()
{
	clear_terminal();
	printf("Going to print all file names and sizes in bytes\n");
	wait(WAIT_6);

	dentry_t dir;
	dir_open(&dir);
	dir_read();
	dir_close();
	return 1;
}

int combined_test_cp2() {
	rtc_open();

	TEST_OUTPUT("Terminal Test: ", test_terminal());
	wait(WAIT_4);

	TEST_OUTPUT("RTC TEST 1: ", test_rtc1());
	wait(WAIT_4);

	TEST_OUTPUT("RTC TEST 2: ", test_rtc2());
	wait(WAIT_4);

	TEST_OUTPUT("Filesys Test (Long File): ", test_filesys_contents_long());
	wait(WAIT_4);

	TEST_OUTPUT("Filesys Test (Text File): ", test_filesys_contents_text());
	wait(WAIT_4);

	TEST_OUTPUT("Filesys Test (Nontext File): ", test_filesys_contents_nontext());
	wait(WAIT_4);

	TEST_OUTPUT("Filesys Directory Test: ", test_filesys_dir_read());
	wait(WAIT_4);

	return 1;
}

/* Checkpoint 3 tests */
int dir_syscall_open_close_test(){
	/*
	printf("Initial FD struct after init\n");
	debug_print_fd();
	wait_and_clear(10);

	printf("double file open & rtc test\n");
	printf("TEST: direct call to open, filename = rtc : ");
	printf("%d\n", open((uint8_t*)"rtc"));
	printf("TEST: direct call to open again, filename = rtc : ");
	printf("%d\n", open((uint8_t*)"rtc"));
	debug_print_fd();
	wait_and_clear(10);

	printf("file open test\n");
	printf("TEST: direct call to open, filename = ls : ");
	printf("%d\n", open((uint8_t*)"ls"));
	printf("TEST: direct call to open, filename = LMAO : ");
	printf("%d\n", open((uint8_t*)"LMAO"));
	debug_print_fd();
	wait_and_clear(10);

	printf("dir open test test\n");
	printf("TEST: direct call to open, dir = . : ");
	printf("%d\n", open((uint8_t*)"."));
	printf("TEST: direct call to open, dir = ./LMAO : ");
	printf("%d\n", open((uint8_t*)"./LMAO"));
	debug_print_fd();
	wait_and_clear(10);

	printf("open too many files test (following files may fail or succeed)\n");
	printf("TEST: direct call to open, filename = cat : %d\n", open((uint8_t*)"cat"));
	printf("TEST: direct call to open, filename = counter : %d\n", open((uint8_t*)"counter"));
	printf("TEST: direct call to open, filename = hello : %d\n", open((uint8_t*)"hello"));
	printf("TEST: direct call to open, filename = rtc : %d\n", open((uint8_t*)"rtc"));
	printf("TEST: direct call to open, filename = . : %d\n", open((uint8_t*)"."));
	printf("TEST: direct call to open, filename = ls : %d\n", open((uint8_t*)"ls"));
	printf("\n These should fail\n");
	printf("TEST: direct call to open, filename = pingpong : %d\n", open((uint8_t*)"pingpong"));
	printf("TEST: direct call to open, filename = shell : %d\n", open((uint8_t*)"shell"));
	debug_print_fd();
	wait_and_clear(10);

	printf("close tests\n");
	printf("TEST: direct call to open, filename = cat : %d\n", open((uint8_t*)"cat"));
	printf("TEST: direct call to open, filename = counter : %d\n", open((uint8_t*)"counter"));
	printf("TEST: direct call to open, filename = hello : %d\n", open((uint8_t*)"hello"));
	printf("TEST: direct call to close, filename = cat : %d\n", close(2));
	printf("TEST: direct call to close, filename = stdin : %d\n", close(3));
	printf("TEST: direct call to close, filename = stdout : %d\n", close(1));
	debug_print_fd();
	wait_and_clear(10);

	printf("attempt to open file in fd gap (after a close)\n");
	printf("TEST: direct call to open, filename = rtc : %d\n", open((uint8_t*)"rtc"));
	debug_print_fd();
	*/
	return 1;
}
int dir_syscall_read_write_test(){		// assumes open/close works (which they do)
	int i = 1;
/*
	printf("Terminal write tests");
	printf("terminal write test: big text, small buffer \n");
	i = write(1, "asdf1234 asdf1234", 15);
	printf("RESULT = %d\n\n", i);

	printf("terminal write test: small text, big buffer \n");
	i = write(1, "asdf124", 15);
	printf("RESULT = %d\n\n", i);

	printf("terminal write test: no buffer \n");
	i = write(1, "asdf1234 asdf1234", 0);
	printf("RESULT = %d\n\n", i);

	printf("terminal write test: no text \n");
	i = write(1,"", 15);
	printf("RESULT = %d\n\n", i);
	wait_and_clear(6);
*/
/*
	printf("write test, write to all fd's\n");
	debug_print_fd();
	i = write(0,"asdf\n", 4);
	printf("fd0 -> %d\n", i);
	i = write(1,"This should echo this text!\n", 4);		// also writes
	printf("fd1 -> %d\n", i);
	i = write(2,"asdf\n", 4);
	printf("fd2 -> %d\n", i);
	i = write(3,"asdf\n", 4);
	printf("fd3 -> %d\n", i);
	wait_and_clear(10);

	debug_print_fd();
	i = write(4,"asdf\n", 4);
	printf("fd4 -> %d\n", i);
	i = write(5,"asdf\n", 4);
	printf("fd5 -> %d\n", i);
	i = write(6,"asdf\n", 4);
	printf("fd6 -> %d\n", i);
	i = write(7,"asdf\n", 4);
	printf("fd7 -> %d\n", i);
*/
	char buf[100];
	printf("read test beginning\n");

	/*
	printf("type something to place in buffer: ");
	printf("read from fd0 = %d\n", read(0, buf, 40));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	printf("read from fd1 (stdout, should fail) = %d\n", read(1, buf, 5));
	*/
	/*
	i = open((uint8_t*)"rtc");
	printf("read from rtc = %d\n", read(i, buf, 1));
	printf("read from rtc = %d\n", read(i, buf, 1));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	*/
	/*
	i = open((uint8_t*)"frame0.txt");
	debug_print_fd();
	printf("read 5 from frame0 = %d\n", read(i, buf, 5));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	clear_buf(buf, 100);
	printf("read 10 from frame0 = %d\n", read(i, buf, 10));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	clear_buf(buf, 100);
	printf("read 100 from frame0 = %d\n", read(i, buf, 100));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	return i;
	*/

	// overflow/end of file check
	/*
	i = open((uint8_t*)"frame0.txt");
	clear_buf(buf, 100);
	debug_print_fd();
	printf("read 100 from frame0 = %d\n", read(i, buf, 100));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	clear_buf(buf, 100);
	printf("read 100 from frame0 = %d\n", read(i, buf, 100));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	clear_buf(buf, 100);
	printf("read 100 from frame0 = %d\n", read(i, buf, 100));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	clear_buf(buf, 100);
	printf("read 100 from frame0 = %d\n", read(i, buf, 100));
	printf("buf result = %s\n\n", (uint8_t*)buf);
	*/

	// Read from dir check
	i = open ((uint8_t*)".");
	printf("read 1 from . = %d\n", read(i, buf, 1));
	printf("buf result = %s\n", (uint8_t*)buf);
	clear_buf(buf, 100);

	return i;
}


int test_execute(){
	uint8_t command[8] = "hello 67";
	int exe = execute(command);
	if(exe == -1){
		printf("File Not Found");
		return 0;
	}
	else if(exe == -4){
		printf("File is not an executable");
		return 0;
	}
	if(exe < 0){
		printf ("Error", exe);
		return 0;
	}
	return 1;
}

int test_execute2(){
	clear_terminal();
	if(test_execute_asm())
		return 0;
	return 1;
}
/* Checkpoint 4 tests */
/* Checkpoint 5 tests */


/* Test suite entry point */
void launch_tests(){
	//clear_terminal();
	//TEST_OUTPUT("sys_call_file_ops test: ", dir_syscall_read_write_test());
	// TEST_OUTPUT("idt test", idt_test());
	// TEST_OUTPUT("keyboard test", test_keyboard());
	// Both are designed to throw exception so only one will work at a time.
	// TEST_OUTPUT("divide by zero test", test_div_by_zero());
	// TEST_OUTPUT("paging test", test_paging());
	//TEST_OUTPUT("CP2 combined test: ", combined_test_cp2());
	TEST_OUTPUT("Execute Test", test_execute2());


	//clear_terminal();
	//printf("Test keyboard inputs and screen clearing: ");
}
