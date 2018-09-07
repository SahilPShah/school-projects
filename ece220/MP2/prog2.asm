;Sahil Shah
;sahils2
;MP2-stack calculator
;implement a calculator using stack arithmetic
;compute addition, subtraction, multiplication, division, and powers
;R3 and R4 will be used as the operands
;R0 will be have the output of each operation and also contain the value inputted from the keyboard and outputted to the display
;R3 will also be the value to be printed when the final calculation has been made
;R5 will be the status of the expression (-1 --> invalid expression)
;R1 and R2 will be used as temporary registers
;R7 is used during Jump Subroutine and for all TRAP instructions
;R6 is currently unused
.ORIG x3000
	;initialize registers
		AND R0,R0,#0		;
		AND R1,R1,#0		;
		AND R2,R2,#0		;
		AND R3,R3,#0		;
		AND R4,R4,#0		;
		AND R5,R5,#0		;
		AND R6,R6,#0		;
		AND R7,R7,#0		;
		LD R1, EQ_SIGN		;
		NOT R1,R1			;
		ADD R1,R1,#1		;
		ST R1,NEG_EQ		;store the negative of the ASCII value of the '='
	INPUT
		GETC				;input will be stored in R0
		OUT					;echo input
		LD R1, NEG_EQ		;
		AND R2,R2,#0		;clear R2
		ADD R2,R2,R0		;copy R0 into R2
		ADD R1,R2,R1		;subtract R2, R1
		BRnp NOT_EQUALS		;
		JSR isEMPTY			;check if empty
		BRnp INV_EXP		;branch if invalid expression
		ADD R5,R5,#0		;check if invalid expression somewhere else
		BRn INV_EXP			;branch if invalid expression
		JSR POP				;POP value
		JSR isEMPTY			;check if empty
		BRz	INV_EXP			;branch if NOT EMPTY
		AND R3,R3,#0		;
		ADD R3,R3,R0		;put result to be printed into R3
		AND R5,R5,#0		;
		ADD R5,R5,R3		;put the result in R5
		LD R1,NEG_0			;
		ADD R5,R5,R1		;convert ASCII to Hex
		JSR PRINT_HEX		;print the hex notation of the result
		BRnzp PROG_FIN		;branch to end of program
	INV_EXP
		LEA R0,INVALID		;
		PUTS				;output invalid message
		BRnzp PROG_FIN		;branch to end of program
	NOT_EQUALS
		LD R1,CHAR_0		;
		AND R2,R2,#0		;
		ADD R2,R2,R0		;copy inputted char into R2
		NOT R1,R1
		ADD R1,R1,#1		;negate '0'
		ADD R1,R2,R1		;subtract R2 and R1
		BRn	NOT_NUM			;branch if ascii character inputted is less than '0'
		LD R1,CHAR_9		;
		AND R2,R2,#0		;
		ADD R2,R2,R0		;copy inputted char into R2
		NOT R1,R1
		ADD R1,R1,#1		;negate '9'
		ADD R1,R2,R1		;subtract R2 and R1
		BRp	NOT_NUM			;branch if ascii character is more than '9'. If the program gets
							;past here, the char typed must be a number
		JSR PUSH
		BRnzp INPUT
	NOT_NUM
		ST R0, Save_OP		;save the non-number ascii character to memory
		JSR isEMPTY			;check if stack is empty
		BRn STACK_EMPTY		;branch if stack is empty (R6 == -1)
		JSR POP				;pop a value
		AND R4,R4,#0
		ADD R4,R4,R0		;put 1st popped value into R4 (operand 2)
		JSR isEMPTY
		BRn STACK_EMPTY		;branch if the stack is empty (R6 == -1)
		JSR POP
		AND R3,R3,#0
		ADD R3,R3,R0		;put 2nd popped value into R3 (operand 1)
		LD R0, Save_Op		;
		JSR EVALUATE		;evaluate the expression
		ADD R5,R5,#0
		BRn INPUT			;if R5 is negative, the expression is invalid, do not PUSH
		AND R0,R0,#0		;
		ADD R0,R0,R6		;put the result of the operation in R0
		LD R1,CHAR_0		;
		ADD R0,R0,R1		;convert back to ASCII representation
		JSR PUSH			;PUSH the result onto the stack
		BRnzp INPUT			;go back to receive another input
	STACK_EMPTY
		AND R5,R5,#0
		ADD R5,R5,#-1
		BRnzp INPUT
	PROG_FIN
		HALT
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;saved operation
Save_OP				.BLKW  #1							;

;data
CHAR_0				.FILL x0030							;
CHAR_9				.FILL x0039							;
INVALID 			.STRINGZ 	"Invalid Expression"	;
MINUS_SIGN			.FILL x002D							;subtraction sign
PLUS_SIGN			.FILL x002B							;plus sign
MULT_SIGN			.FILL x002A							;multiplication sign
DIV_SIGN			.FILL x002F							;division sign
EXP_SIGN			.FILL x005E							;power sign
EQ_SIGN				.FILL x003D							;equal sign
NEG_MINUS			.BLKW #1							;
NEG_PLUS			.BLKW #1							;
NEG_MULT			.BLKW #1							;
NEG_DIV				.BLKW #1							;
NEG_EXP				.BLKW #1							;
NEG_EQ				.BLKW #1							;
NEG_0				.BLKW #1							;
ASCII_NUM  			.FILL x0030							;
ASCII_LET  			.FILL x0037							;


isEMPTY
	;R1 and R2 will hold TSP and STACK_START
	;if R1 is zero then the the TSP points to the Start of the stack and the stack is empty
	;if R1 != 0, the stack is not empty
	;OUT: R6 == -1 if stack is empty
		ST R1,EMPTY_SaveR1			;
		ST R2,EMPTY_SaveR2			;
		AND R6,R6,#0				;
		LD R1,STACK_TOP				;
		LD R2,STACK_START			;
		NOT R2,R2					;
		ADD R2,R2,#1				;TSP-STACK_START, if TSP == STACK_START/ TSP<STACK_START, stack is not empty
		ADD R1,R2,R1				;
		BRn EMPTY_FIN				;branch if TSP>STACK_START
		ADD R6,R6,#-1				;
	EMPTY_FIN
		LD R1,EMPTY_SaveR1			;
		LD R2,EMPTY_SaveR2			;
		ADD R6,R6,#0				;
		RET							;end of subroutine


PRINT_HEX
	;R3 will hold the value to be printed in ASCII, need to convert back to binary
	;R1 will be the digit counter...will be set to 4. Every 4 bits will be converted into a digit and there are only 4 digits
	;R2 will be the bit counter...will be set to 4. Each digit requires 4 bits
	;R0 will be the digit itself.
	;R4 is a temporary register
	;save registers. R7 is saved later (before the OUT trap)
		ST R0,PRINT_SAVE_R0 ;
		ST R1,PRINT_SAVE_R1 ;
		ST R2,PRINT_SAVE_R2 ;
		ST R3,PRINT_SAVE_R3 ;
		ST R4,PRINT_SAVE_R4 ;
		ST R5,PRINT_SAVE_R5 ;
		ST R6,PRINT_SAVE_R6 ;

;convert back to decimal representation
		LD R1,NEG_0			;
		ADD R3,R3,R1		;
;initialize
	 	AND R0,R0,#0		;initialize the digit
	 	AND R1,R1,#0  		;initialize the digit counter
	 	ADD R1,R1,#4  		;set the digit counter
OUT_LOOP   	BRz FIN  		;jump to finish if 4 digits have been produced
	 	AND R2,R2,#0 		;reset the bit counter
	        ADD R2,R2,#4 	;set the bit counter to 4
LOOP       	BRz DIGIT 		;jump only if 4 bits for the digit have been retrieved
	        ADD R0,R0,R0	;shift digit left
		ADD R3,R3,#0 		;change CC to analyze R3
		BRzp POS 			;jump if positive(MSB == 0);
		ADD R0,R0,#1 		;ADD 1 to digit.
POS        	ADD R3,R3,R3 	;shift R3 to prepare to check next bit
		ADD R2,R2,#-1 		;decrement bit counter
		BRnzp LOOP
DIGIT      	ADD R4,R0,#-9 	;subract 9
		BRp CHAR 			;jump if digit is >=10
		LD R4,ASCII_NUM 	;put the ASCII val of 0 in R4
		ADD R0,R0,R4 		;This will store the ASCII value of a number into R0
		ST R7,PRINT_SAVE_R7	;save R7
		OUT
		LD R7,PRINT_SAVE_R7	;
		AND R0,R0,#0 		;clear digit
		ADD R1,R1,#-1 		;decrement digit counter
		BRnzp OUT_LOOP
CHAR
		LD R4,ASCII_LET 	;put the ASCII val of A-10 in R4
		ADD R0,R0,R4 		;This will store the ASCII value of a Letter (A-F) into R0
		ST R7,PRINT_SAVE_R7 ;save R7
		OUT
		LD R7,PRINT_SAVE_R7 ;restore R7
		AND R0,R0,#0 		;clear digit
		ADD R1,R1,#-1 		;decrement digit counter
		BRnzp OUT_LOOP
;restore registers
FIN
		LD R0,PRINT_SAVE_R0
 		LD R1,PRINT_SAVE_R1
 		LD R2,PRINT_SAVE_R2
 		LD R3,PRINT_SAVE_R3
		LD R4,PRINT_SAVE_R4
		LD R5,PRINT_SAVE_R5
 		LD R6,PRINT_SAVE_R6
		RET 				;return

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard (the operator)
;R6 - current numerical output
;use R1 as a temporary register
;values required by the other math subroutines should already be in R3 and R4
;R5 will be set to -1 if the computation is invalid (divide by zero OR operator is invalid)
EVALUATE
		ST R7,EVAL_SaveR7		;save R7
		ST R0,EVAL_SaveOP		;save the operator
		ST R1,EVAL_SaveR1		;save R1
		ST R6,EVAL_SaveR6		;save R6
	;negate the operators
		LD R1,MINUS_SIGN		;load minus sign
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1, NEG_MINUS		;store in memory
		LD R1,PLUS_SIGN			;load plus sign
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1, NEG_PLUS 		;store in memory
		LD R1,MULT_SIGN			;load multiplication sign
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1,NEG_MULT			;store in memory
		LD R1,DIV_SIGN			;load minus sign
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1, NEG_DIV  		;store in memory
		LD R1,EXP_SIGN			;load exponent sign
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1,NEG_EXP			;store in memory
		LD R1,CHAR_0			;load '0'
		NOT R1,R1				;
		ADD R1,R1,#1			;complete negation
		ST R1,NEG_0				;store in memory
	;clear registers
		AND R6,R6,#0			;
	;check which operator was inputted
	;check minus sign
		LD R1,NEG_MINUS			;
		ADD R0,R0,R1			;
		BRnp NOT_MINUS			;
		JSR MIN					;
		BRnzp EVAL_FIN			;
NOT_MINUS						;check plus sign
		LD R0,EVAL_SaveOP		;
		LD R1,NEG_PLUS			;
		ADD R0,R0,R1			;
		BRnp NOT_PLUS			;
		JSR PLUS				;
		BRnzp EVAL_FIN			;
NOT_PLUS						;check multiplication sign
		LD R0,EVAL_SaveOP		;
		LD R1,NEG_MULT			;
		ADD R0,R0,R1			;
		BRnp NOT_MULT			;
		JSR MULT				;
		BRnzp EVAL_FIN			;
NOT_MULT						;check divide sign
		LD R0,EVAL_SaveOP		;
		LD R1,NEG_DIV			;
		ADD R0,R0,R1			;
		BRnp NOT_DIV			;
		JSR DIV					;
		BRnzp EVAL_FIN			;
NOT_DIV							;check exponent
		LD R0,EVAL_SaveOP		;
		LD R1,NEG_EXP			;
		ADD R0,R0,R1			;
		BRnp NOT_EXP			;
		JSR EXP					;
		BRnzp EVAL_FIN			;
NOT_EXP							;if it isn't one of the operations allowed by this program
		ADD R5,R5,#-1			;status register
EVAL_FIN
		LD R7,EVAL_SaveR7		;
		ADD R6,R0,#0			;put the result of the operation in R6
		RET						;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;need to convert numbers from ascii rep to binary representation
PLUS
	LD R1, NEG_0			;
	ADD R3,R3,R1			;convert to binary
	ADD R4,R4,R1			;convert to binary
	ADD R0,R3,R4			;add the two operands
	RET

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN
	LD R1, NEG_0			;
	ADD R3,R3,R1			;convert to binary
	ADD R4,R4,R1			;convert to binary
	NOT R4,R4 				;invert R1
	ADD R4,R4,#1			;add one to R1 to complete twos complement negation
	ADD R0,R3,R4 			;compute R2-R1 = R0
	RET						;end of subroutine

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;R0 <--- R3-R4
MULT
		ST R3,MULT_SaveR3		;
		ST R4,MULT_SaveR4		;
		LD R1, NEG_0			;
		ADD R3,R3,R1			;convert to binary
		ADD R4,R4,R1			;convert to binary
		AND R0,R0,#0			;reset R0
		ADD R3,R3,#0			;set CC to R2 to check if zero or not
	MULT_LOOP
		ADD R0,R0,R4			;perform addition
		ADD R3,R3,#-1			;decrement R2
		BRp MULT_LOOP			;loop back to top
		LD R3,MULT_SaveR3		;
		LD R4,MULT_SaveR4		;
		RET						;End of subroutine

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;use R6 as a negative/positive answer bit (0 == negative, +-2 == positive)
;R0 <--- R3/R4
DIV
		LD R1, NEG_0			;
		ADD R3,R3,R1			;convert to binary
		ADD R4,R4,R1			;convert to binary
		ST R6,DIV_SaveR6		;
		AND R6,R6,#0			;
	;check the sign of the 2 operands
		ADD R3,R3,#0			;check first operand
		BRn NEG_NUM				;
		ADD R6,R6,#1			;
		BRnzp OP_TWO			;
	NEG_NUM
		NOT R3,R3
		ADD R3,R3,#1
		ADD R6,R6,#-1
	OP_TWO
		ADD R4,R4,#0			;check the 2nd operand
		BRn NEG_TWO
		ADD R6,R6,#1			;
		BRnzp START_DIV
	NEG_TWO
		NOT R4,R4
		ADD R4,R4,#1			;
		ADD R6,R6,-1			;

	START_DIV
		AND R0,R0,R0			;reset R0
		NOT R4,R4				;invert R4
		ADD R4,R4,#1			;complete negation (2s complement)
		DIV_LOOP
		ADD R0,R0,#1			;count how many times R3 can be subtracted from R4
		ADD R3,R3,R4			;subtract R4 from R3
		BRn NEG					;quotient should be decremented, then go to RET
		BRz DIV_FIN				;if R3 becomes zero, division is complete, go to RET
		BRp DIV_LOOP			;repeat subtraction
	NEG
		ADD R0,R0,#-1			;decrement R0 because last subtraction did not stay greater than zero (R1 > R2 at that time)
	DIV_FIN
		ADD R6,R6,#0			;
		BRnp POS_OUT
		NOT R0,R0
		ADD R0,R0,#1			;make the quotient negative if one of the inputs was negative
	POS_OUT
		LD R5,Div_SaveR6		;
		RET						;end of subroutine

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
;use MULT subroutine to implement this one
;R3^R4
;copy R3 into R4 and use R5 as a counter
;SAVE R5
EXP
			ST R5,EXP_SaveR5		;save R5
			St R7,EXP_SaveR7		;save R7
			LD R1, NEG_0			;
			ADD R3,R3,R1			;convert to binary
			ADD R4,R4,R1			;convert to binary
			ADD R5,R4,#-1			;
			ADD R4,R3,#0			;
			LD R1,CHAR_0
			ADD R3,R3,R1			;convert back to ASCII
			ADD R4,R4,R1			;convert back to ASCII
EXP_LOOP	JSR MULT				;multiply R3 and R3, output in R0
			LD R1,CHAR_0			;
			ADD R3,R0,R1			;load the ASCII value of the char stored in R0 into R3
			ADD R5,R5,#-1			;
			BRp EXP_LOOP			;
			LD R5,EXP_SaveR5		;reload R5
			LD R7,EXP_SaveR7		;reload R7
			ADD R0,R3,#0			;final result in R0
			LD R1,NEG_0				;
			ADD R0,R0,R1			;convert from ASCII to hex
			RET




;stack data
STACK_END			.FILL x3FF0							;
STACK_START			.FILL x4000							;
STACK_TOP			.FILL x4000							;



;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH
		ST R3, PUSH_SaveR3	;save R3
		ST R4, PUSH_SaveR4	;save R4
		AND R5, R5, #0		;
		LD R3, STACK_END	;
		LD R4, STACk_TOP	;
		ADD R3, R3, #-1		;
		NOT R3, R3			;
		ADD R3, R3, #1		;
		ADD R3, R3, R4		;
		BRz OVERFLOW		;stack is full
		STR R0, R4, #0		;no overflow, store value in the stack
		ADD R4, R4, #-1		;move top of the stack
		ST R4, STACK_TOP	;store top of stack pointer
		BRnzp DONE_PUSH		;
	OVERFLOW
		ADD R5, R5, #1		;
	DONE_PUSH
		LD R3, PUSH_SaveR3	;
		LD R4, PUSH_SaveR4	;
		RET


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP
		ST R3, POP_SaveR3	;save R3
		ST R4, POP_SaveR4	;save R3
		AND R5, R5, #0		;clear R5
		LD R3, STACK_START	;
		LD R4, STACK_TOP	;
		NOT R3, R3		;
		ADD R3, R3, #1		;
		ADD R3, R3, R4		;
		BRz UNDERFLOW		;
		ADD R4, R4, #1		;
		LDR R0, R4, #0		;
		ST R4, STACK_TOP	;
		BRnzp DONE_POP		;
	UNDERFLOW
		ADD R5, R5, #1		;
	DONE_POP
		LD R3, POP_SaveR3	;
		LD R4, POP_SaveR4	;
		RET

;subroutine register saves


EMPTY_SaveR1		.BLKW #1				;
EMPTY_SaveR2		.BLKW #1				;

EXP_SaveR7			.BLKW #1				;
PLUS_SaveR7			.BLKW #1				;
MINUS_SaveR7		.BLKW #1				;
DIV_SaveR7			.BLKW #1				;
MULT_SaveR7			.BLKW #1				;
EVAL_SAVER7			.BLKW #1				;
PRINT_SAVE_R7 		.BLKW #1				;

MULT_SaveR3			.BLKW #1				;
MULT_SaveR4			.BLKW #1				;

EXP_SaveR5			.BLKW #1				;

PUSH_SaveR3			.BLKW #1				;
PUSH_SaveR4			.BLKW #1				;

DIV_SaveR6			.BLKW #1  				;

EVAL_SaveOP			.BLKW #1				;
EVAL_SaveR1			.BLKW #1				;
EVAL_SaveR6			.BLKW #1				;

POP_SaveR3			.BLKW #1				;
POP_SaveR4			.BLKW #1				;

PRINT_SAVE_R0 		.BLKW #1				;
PRINT_SAVE_R1 		.BLKW #1				;
PRINT_SAVE_R2 		.BLKW #1				;
PRINT_SAVE_R3 		.BLKW #1				;
PRINT_SAVE_R4 		.BLKW #1				;
PRINT_SAVE_R5 		.BLKW #1				;
PRINT_SAVE_R6 		.BLKW #1				;


.END
