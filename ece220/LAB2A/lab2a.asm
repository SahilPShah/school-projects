.ORIG x3000
; Write code to read in characters and echo them
; till a newline character is entered.
;enter input
;if '(' then push to stack
;if ')' the pop from stack
;if stack is empty when trying to pop, the expression was not balanced
;run until \n is typed (ENTER)

;need to find NEG_OPEN, NEG_CLOSE, NEG_NL, NEG_TSP
INVALID         .STRINGZ "Invalid Expression"
VALID 			.STRINGZ "Valid Expression!"
OPEN            .FILL x0028
CLOSE           .FILL x0029
SPACE           .FILL x0020
CHAR_RETURN     .FILL x000D		;char when ENTER is pressed
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;if ( push onto stack if ) pop from stack and check if popped value is (
;input - R0 holds the input
;output - R4 set to -1 if unbalanced. else not modified.
IS_BALANCED
;save R7

	ST R7, SaveR7_IsBalanced

;initilize registers

	AND R0,R0,#0
	AND R1,R1,#0
	AND R2,R2,#0
	AND R3,R3,#0

;get inverses of OPEN, CLOSE, and NEW_LINE

    LD R4, OPEN
    NOT R4, R4
    ADD R4, R4, #1
    ST R4, NEG_OPEN
    LD R4, CLOSE
    NOT R4, R4
    ADD R4, R4, #1
    ST R4, NEG_CLOSE
    LD R4, CHAR_RETURN
    NOT R4, R4
    ADD R4, R4, #1
    ST R4, NEG_CR
    LD R4, STACK_TOP
    NOT R4, R4
    ADD R4, R4, #1
    ST R4, NEG_TSP

;get input

INPUT
    GETC               	;input will be stored in R0
    OUT					;echo char
    ADD R1,R1,R0        ;copy input into R0
    LD R2,NEG_OPEN      ;
    ADD R1,R1,R2        ;
    BRnp NOT_OPEN       ;branch if not '('
    JSR PUSH            ;push the char if it is '('
    BRnzp INPUT         ;jump back to get a new input
NOT_OPEN
    AND R1,R1,#0        ;clear R1
    ADD R1,R1,R0        ;put input into R1
    LD R2,NEG_CR        ;
    ADD R1,R1,R2        ;
    BRnp NOT_CR         ;branch if not NL
	AND R3,R3,R3		;
	BRn INVALID_EXP		;if R3 is set to -1, it is invalid.
    LD R3,STACK_TOP     ;load stack pointer
    LD R4,NEG_TSP       ;inverse of x4000
    ADD R3,R3,R4        ;
    BRnp INVALID_EXP    ;branch if not empty
	LEA R0, Valid		;load starting address of "Valid Expression!"
	PUTS				;
	BRnzp DONE			;
INVALID_EXP
    LEA R0,INVALID      ;load starting address of "Invalid Expression"
    PUTS                ;
    BRnzp DONE          ;
NOT_CR
    AND R1,R1,#0        ;clear R1
    ADD R1,R1,R0        ;put R0 into R1
    LD R2,NEG_CLOSE 	;
	ADD R1,R1,R2		;
    BRz STACK_EMPTY    	;branch if not ')'. If no, it must be an invalid expression
	ADD R3,R3,#-1		;invalid expression
STACK_EMPTY
	LD R3,STACK_TOP     ;load stack pointer
    LD R4,NEG_TSP       ;inverse of x4000
    ADD R3,R3,R4        ;
    BRnp NOT_EMPTY    	;if stack is not empty ==> POP
	ADD R3,R3,#-1		;invalid expression
NOT_EMPTY
    JSR POP             ;POP value from the stack
    BRnzp INPUT         ;go back to top for next input
DONE
    LD R7,SaveR7_IsBalanced
    RET


NEG_OPEN              .BLKW #1
NEG_CLOSE             .BLKW #1
NEG_CR                .BLKW #1
NEG_TSP               .BLKW #1
SaveR7_IsBalanced     .BLKW #1

;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH
        ST R3, PUSH_SaveR3      ;save R3
        ST R4, PUSH_SaveR4      ;save R4
        AND R5, R5, #0          ;
        LD R3, STACK_END        ;
        LD R4, STACk_TOP        ;
        ADD R3, R3, #-1         ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz OVERFLOW            ;stack is full
        STR R0, R4, #0          ;no overflow, store value in the stack
        ADD R4, R4, #-1         ;move top of the stack
        ST R4, STACK_TOP        ;store top of stack pointer
        BRnzp DONE_PUSH         ;
OVERFLOW
        ADD R5, R5, #1          ;
DONE_PUSH
        LD R3, PUSH_SaveR3      ;
        LD R4, PUSH_SaveR4      ;
        RET                     ;

PUSH_SaveR7	.BLKW #1	;
PUSH_SaveR3     .BLKW #1        ;
PUSH_SaveR4     .BLKW #1        ;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;

POP
        ST R3, POP_SaveR3       ;save R3
        ST R4, POP_SaveR4       ;save R3
        AND R5, R5, #0          ;clear R5
        LD R3, STACK_START      ;
        LD R4, STACK_TOP        ;
        NOT R3, R3              ;
        ADD R3, R3, #1          ;
        ADD R3, R3, R4          ;
        BRz UNDERFLOW           ;
        ADD R4, R4, #1          ;
        LDR R0, R4, #0          ;
        ST R4, STACK_TOP        ;
        BRnzp DONE_POP          ;
UNDERFLOW
        ADD R5, R5, #1          ;
DONE_POP
        LD R3, POP_SaveR3       ;
        LD R4, POP_SaveR4       ;
        RET

POP_SaveR7
POP_SaveR3      .BLKW #1        ;
POP_SaveR4      .BLKW #1        ;
STACK_END       .FILL x3FF0     ;
STACK_START     .FILL x4000     ;
STACK_TOP       .FILL x4000     ;

.END
