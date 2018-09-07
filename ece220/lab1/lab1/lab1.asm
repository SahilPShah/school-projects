.ORIG x3000
;R3 will hold the value to be printed
;R1 will be the digit counter...will be set to 4. Every 4 bits will be converted into a digit and there are only 4 digits
;R2 will be the bit counter...will be set to 4. Each digit requires 4 bits
;R0 will be the digit itself.
;R4 is a temporary register
;initialize
           AND R0,R0,#0 ;initialize the digit
           AND R1,R1,#0  ;initialize the digit counter
           LD R3, NUM  ;load x0035 into R3 --> value to be printed
           ADD R1,R1,#4  ;set the digit counter
OUT_LOOP   BRz DONE  ;jump to finish if 4 digits have been produced
           AND R2,R2,#0 ;reset the bit counter
           ADD R2,R2,#4 ;set the bit counter to 4
LOOP       BRz DIGIT ;jump only if 4 bits for the digit have been retrieved
           ADD R0,R0,R0;shift digit left
           ADD R3,R3,#0 ;change CC to analyze R3
           BRzp POS ;jump if positive(MSB == 0);
           ADD R0,R0,#1 ;ADD 1 to digit.
POS        ADD R3,R3,R3 ;shift R3 to prepare to check next bit
           ADD R2,R2,#-1 ;decrement bit counter
           BRnzp LOOP
DIGIT      ADD R4,R0,#-9 ;subract 9
           BRp CHAR ;jump if digit is >=10
           LD R4,ASCII_NUM ;put the ASCII val of 0 in R4
           ADD R0,R0,R4 ;This will store the ASCII value of a number into R0
           OUT
           AND R0,R0,#0 ;clear digit
           ADD R1,R1,#-1 ;decrement digit counter
           BRnzp OUT_LOOP
CHAR       LD R4,ASCII_LET ;put the ASCII val of A-10 in R4
           ADD R0,R0,R4 ;This will store the ASCII value of a Letter (A-F) into R0
           OUT
           AND R0,R0,#0 ;clear digit
           ADD R1,R1,#-1 ;decrement digit counter
           BRnzp OUT_LOOP
DONE       HALT
NUM        .FILL xAA45
ASCII_NUM  .FILL x0030
ASCII_LET  .FIll x0037
           .END
