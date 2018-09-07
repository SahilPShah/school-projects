
.ORIG x3000

INIT_REGS
  AND R0, R0, #0
  AND R1, R1, #0
  AND R2, R2, #0
  AND R3, R3, #0
  AND R4, R4, #0
  AND R5, R5, #0
  AND R6, R6, #0

  LD R6, INPUT_ADDRESS
  LDR R6, R6, #0

;TODO
LOOP                 ;R1 will be the counter
                     ;R2 will keep track of the digits tracked
    ADD R2,R2,#15
    ADD R6,R6, #0    ;
    BRzp POS         ;shift if the MSB == 0
    ADD R1,R1,#1     ;increment R1
POS
    ADD R6,R6,R6     ;bit shift left
    ADD R2,R2,#-1
    BRp LOOP       ;jump back to start if there are still more
    DONE




; END TODO

	STI R0, OUTPUT_VALUE

HALT

; Memory definition.
INPUT_ADDRESS .FILL x5000
OUTPUT_VALUE .FILL x5000

.END
