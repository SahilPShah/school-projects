;number to print in base 7 is in R3.
;it will be positive.
.ORIG x3000

;your program starts here

;initialize
	AND R0,R0,#0		;
	AND R1,R1,#0		;
	AND R3,R3,#0		;
	AND R4,R4,#0		;

	ADD R3,R3,#9		;store test case 9 into R3
	ADD R4,R4,#7		;store 7 into R3
DIVIDE
	JSR DIV				;divide R3/7....quotient in R0, remainder in R1
	AND R3,R3,#0		;
	ADD R3,R0,R3		;put the quotient into R3
	AND R0,R0,#0		;
	ADD R0,R1,R0		;put the remainder into R0
	JSR PUSH			;push quotient onto the stack
	AND R3,R3,R3		;change CC to R3
	BRnp DIVIDE			;
	JSR POP				;pop value
	DONE				;end of programS











ASCII_0 .FILL x30

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;check if the stack is empty
;OUT: R5 (-1 if stack is empty)
isEmpty
	AND R5,R5,#0
	LD R1,STACK_START
	LD R2 STACK_TOP
	NOT R1,R1
	ADD R1,R1,#1
	ADD R1,R2,R1
	BRn NOT_EMPTY		;stack is not empty
	ADD R5,R5,#-1		;set R5 to -1
NOT_EMPTY
	DONE








;input R3, R4 (R3/R4)
;out R0-quotient, R1-remainder
DIV
	ST R2,DIV_R2
	ST R3,DIV_R3
	NOT R2,R4
	ADD R2,R2,#1
	ADD R0,R0,#-1
DIV_LOOP
	ADD R0,R0,#1
	ADD R3,R3,R2
	BRzp DIV_LOOP
	ADD R1,R3,R4
	LD R2,DIV_R2
	LD R3,DIV_R3
	RET

DIV_R2 .BLKW #1
DIV_R3 .BLKW #1


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
	NOT R3, R3		;
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


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


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


POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;

.END
