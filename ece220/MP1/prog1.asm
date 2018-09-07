; Sahil Shah, sahils2, ECE 220, BL1
; The code given to you here implements the histogram calculation that
; we developed in class.  In programming studio, we will add code that
; prints a number in hexadecimal to the monitor.
;
; Your assignment for this program is to combine these two pieces of
; code to print the histogram to the monitor.
;
; If you finish your program,
;    ** commit a working version to your repository  **
;    ** (and make a note of the repository version)! **


	.ORIG	x3000		; starting address is x3000


;
; Count the occurrences of each letter (A to Z) in an ASCII string
; terminated by a NUL character.  Lower case and upper case should
; be counted together, and a count also kept of all non-alphabetic
; characters (not counting the terminal NUL).
;
; The string starts at x4000.
;
; The resulting histogram (which will NOT be initialized in advance)
; should be stored starting at x3F00, with the non-alphabetic count
; at x3F00, and the count for each letter in x3F01 (A) through x3F1A (Z).
;
; table of register use in this part of the code
;    R0 holds a pointer to the histogram (x3F00)
;    R1 holds a pointer to the current position in the string
;       and as the loop count during histogram initialization
;    R2 holds the current character being counted
;       and is also used to point to the histogram entry
;    R3 holds the additive inverse of ASCII '@' (xFFC0)
;    R4 holds the difference between ASCII '@' and 'Z' (xFFE6)
;    R5 holds the difference between ASCII '@' and '`' (xFFE0)
;    R6 is used as a temporary register
;

	LD R0,HIST_ADDR      	; point R0 to the start of the histogram

	; fill the histogram with zeroes
	AND R6,R6,#0		; put a zero into R6
	LD R1,NUM_BINS		; initialize loop count to 27
	ADD R2,R0,#0		; copy start of histogram into R2

	; loop to fill histogram starts here
HFLOOP	STR R6,R2,#0		; write a zero into histogram
	ADD R2,R2,#1		; point to next histogram entry
	ADD R1,R1,#-1		; decrement loop count
	BRp HFLOOP		; continue until loop count reaches zero

	; initialize R1, R3, R4, and R5 from memory
	LD R3,NEG_AT		; set R3 to additive inverse of ASCII '@'
	LD R4,AT_MIN_Z		; set R4 to difference between ASCII '@' and 'Z'
	LD R5,AT_MIN_BQ		; set R5 to difference between ASCII '@' and '`'
	LD R1,STR_START		; point R1 to start of string

	; the counting loop starts here
COUNTLOOP
	LDR R2,R1,#0		; read the next character from the string...ready to be printed
	BRz PRINT_HIST		; found the end of the string

	ADD R2,R2,R3		; subtract '@' from the character
	BRp AT_LEAST_A		; branch if > '@', i.e., >= 'A'
NON_ALPHA
	LDR R6,R0,#0		; load the non-alpha count
	ADD R6,R6,#1		; add one to it
	STR R6,R0,#0		; store the new non-alpha count
	BRnzp GET_NEXT		; branch to end of conditional structure
AT_LEAST_A
	ADD R6,R2,R4		; compare with 'Z'
	BRp MORE_THAN_Z         ; branch if > 'Z'

; note that we no longer need the current character
; so we can reuse R2 for the pointer to the correct
; histogram entry for incrementing
ALPHA	ADD R2,R2,R0		; point to correct histogram entry
	LDR R6,R2,#0		; load the count
	ADD R6,R6,#1		; add one to it
	STR R6,R2,#0		; store the new count
	BRnzp GET_NEXT		; branch to end of conditional structure

; subtracting as below yields the original character minus '`'
MORE_THAN_Z
	ADD R2,R2,R5		; subtract '`' - '@' from the character
	BRnz NON_ALPHA		; if <= '`', i.e., < 'a', go increment non-alpha
	ADD R6,R2,R4		; compare with 'z'
	BRnz ALPHA		; if <= 'z', go increment alpha count
	BRnzp NON_ALPHA		; otherwise, go increment non-alpha

GET_NEXT
	ADD R1,R1,#1		; point to next character in string
	BRnzp COUNTLOOP		; go to start of counting loop



PRINT_HIST
	;Sahil Shah, sahils2, ECE 220, BL1
	;this portion of code will print the histogram computed earlier in this program
	;R0 will be used to output values
	;R1 will be the pointer to the histogram data
	;R2 will keep track of the current character in the histogram
	;R3 will be the actual data from the histogram (specified by the PRINT subroutine)
	;R4 will be a counter, set to 27 --- 1 symbol and 26 letters
	;R5 will hold the space symbol
	;R6 will hold the new line char
	;the program from lab1 will be used here as a subroutine to print the data from the histogram

	;initialize registers
		LD R1, HIST_ADDR
		LD R2, ASCII_CHAR
		LD R4, NUM_BINS
		LD R5, SPACE
		LD R6, NEW_LINE
PRINT_LOOP	ADD R0,R2,#0		;load the character into R0
		OUT			;output the character
		ADD R0,R5,#0		;load space into R0
		OUT			;output a space
		LDR R3,R1,#0		;load value into R3
		JSR PRINT		;compute and output the value from the histogram
		ADD R0,R6,#0		;load \n into R0
		OUT			;output \n
		ADD R1,R1,#1    	;increment the histogram pointer
		ADD R2,R2,#1		;update the character to be printed to the next character
		ADD R4,R4,#-1   	;decrement the counter
		BRp PRINT_LOOP	;jump to top
DONE		HALT			; done

PRINT
	;print hex values subroutine. Takes a value in R3 and outputs
	;R3 will hold the value to be printed
	;R1 will be the digit counter...will be set to 4. Every 4 bits will be converted into a digit and there are only 4 digits
	;R2 will be the bit counter...will be set to 4. Each digit requires 4 bits
	;R0 will be the digit itself.
	;R4 is a temporary register
	;save registers. R7 is saved later (before the OUT trap)
		ST R0,SAVE_R0 ;
		ST R1,SAVE_R1 ;
		ST R2,SAVE_R2 ;
		ST R3,SAVE_R3 ;
		ST R4,SAVE_R4 ;
		ST R5,SAVE_R5 ;
		ST R6,SAVE_R6 ;
;initialize
	 	AND R0,R0,#0		;initialize the digit
	 	AND R1,R1,#0  		;initialize the digit counter
	 	ADD R1,R1,#4  		;set the digit counter
OUT_LOOP   	BRz FIN  		;jump to finish if 4 digits have been produced
	 	AND R2,R2,#0 		;reset the bit counter
	        ADD R2,R2,#4 		;set the bit counter to 4
LOOP       	BRz DIGIT 				;jump only if 4 bits for the digit have been retrieved
	        ADD R0,R0,R0			;shift digit left
		ADD R3,R3,#0 		;change CC to analyze R3
		BRzp POS 				;jump if positive(MSB == 0);
		ADD R0,R0,#1 		;ADD 1 to digit.
POS        	ADD R3,R3,R3 		;shift R3 to prepare to check next bit
		ADD R2,R2,#-1 		;decrement bit counter
		BRnzp LOOP
DIGIT      	ADD R4,R0,#-9 		;subract 9
		BRp CHAR 				;jump if digit is >=10
		LD R4,ASCII_NUM 	;put the ASCII val of 0 in R4
		ADD R0,R0,R4 		;This will store the ASCII value of a number into R0
		ST R7,SAVE_R7		;save R7
		OUT
		LD R7,SAVE_R7		;
		AND R0,R0,#0 		;clear digit
		ADD R1,R1,#-1 		;decrement digit counter
		BRnzp OUT_LOOP
CHAR       	LD R4,ASCII_LET 	;put the ASCII val of A-10 in R4
		ADD R0,R0,R4 		;This will store the ASCII value of a Letter (A-F) into R0
		ST R7,SAVE_R7		;save R7
		OUT
		LD R7,SAVE_R7           ;restore R7
		AND R0,R0,#0 		;clear digit
		ADD R1,R1,#-1 		;decrement digit counter
		BRnzp OUT_LOOP
;restore registers
FIN 		LD R0,SAVE_R0
 		LD R1,SAVE_R1
 		LD R2,SAVE_R2
 		LD R3,SAVE_R3
		LD R4,SAVE_R4
		LD R5,SAVE_R5
 		LD R6,SAVE_R6
		RET 			;return



; the data needed by the program
NUM_BINS	.FILL #27		; 27 loop iterations
NEG_AT		.FILL xFFC0		; the additive inverse of ASCII '@'
AT_MIN_Z	.FILL xFFE6		; the difference between ASCII '@' and 'Z'
AT_MIN_BQ	.FILL xFFE0		; the difference between ASCII '@' and '`'
HIST_ADDR	.FILL x3F00     	; histogram starting address
STR_START	.FILL x4000		; string starting address
ASCII_CHAR      .FILL x0040 		; '@' symbol
SPACE		.FILL x0020 		; space
NEW_LINE	.FILL x000A 		; \n
SAVE_R0		.FILL x0000 		;
Save_R1		.FILL x0000		;
SAVE_R2		.FILL x0000 		;
SAVE_R3		.FILL x0000		;
SAVE_R4		.FILL x0000 		;
SAVE_R5		.FILL x0000 		;
SAVE_R6		.FILL x0000 		;
SAVE_R7		.FILL x0000 		;
ASCII_NUM  	.FILL x0030		;
ASCII_LET  	.FIll x0037		;

; for testing, you can use the lines below to include the string in this
; program...
;STR_START	.FILL STRING		; string starting address
;STRING		.STRINGZ "This is a test of the counting frequency code.  AbCd...WxYz."



	; the directive below tells the assembler that the program is done
	; (so do not write any code below it!)

		.END
