
```ASM
P4 EQU 0C0H
P4SW EQU 0BBH
T1MS EQU 0FC18H                  ;1ms timer calculation method in 12T mode is (65536-18432000/12/1000)
HI EQU 29H
LO EQU 0FAH
CIRC EQU 18H
;/* define SFR */

TEST_LED    BIT  P0.0                   ;work LED, flash once per second

;/* define variables */
COUNT DATA 20H                          ;1000 times counter (2 bytes)
TIK DATA 22H
LED_COUNT DATA 24H

;-----------------------------------------------

    ORG     0000H
    LJMP    MAIN
    ORG     000BH
    LJMP    TM0_ISR
	
;-----------------------------------------------
LEDTABLE:	DB 0C0H;0
			DB 0F9H;1
			DB 0A4H;2
			DB 0B0H;3
			DB 099H;4
			DB 092H;5
			DB 082H;6
			DB 0F8H;7
			DB 080H;8
			DB 090H;9
			DB 088H;A
			DB 083H;B
			DB 0C6H;C
			DB 0A1H;D
			DB 086H;E
			DB 08EH;F
;/* main program */
MAIN:
	MOV P4SW, #30H
	MOV DPTR, #LEDTABLE
	MOV R5, #00H
	MOV R6, #00H
	MOV R7, #00H
	ACALL DISPLAY
    MOV     TMOD,#01H                   ;set timer0 as mode1 (16-bit)
    MOV     TL0,#LOW T1MS               ;initial timer0 low byte
    MOV     TH0,#HIGH T1MS              ;initial timer0 high byte
    SETB    TR0                         ;timer0 start running
    SETB    ET0                         ;enable timer0 interrupt
    SETB    EA                          ;open global interrupt switch
	SETB P3.2
	CLR P1.0
	SETB P1.1
	SETB P1.4
	SETB P3.2
	CLR P1.0
	SETB P1.1
	SETB P1.4
    CLR     A
    MOV     COUNT,A
    //MOV     COUNT+1,A   	;initial counter
	MOV     TIK,A
	MOV     LED_COUNT,A
    MOV     LED_COUNT+1,A   	;initial counter
	MOV     LED_COUNT+2,A
	MOV R0,#CIRC
    
	
	SJMP    $

;-----------------------------------------------

;/* Timer0 interrupt routine */
TM0_ISR:
    PUSH    ACC
    PUSH    PSW
    MOV     TL0,#LOW T1MS               ;reload timer0 low byte
    MOV     TH0,#HIGH T1MS              ;reload timer0 high byte
    MOV     A,COUNT
    //ORL     A,COUNT+1                   ;check whether count(2byte) is equal to 0
    JNZ     SKIP
	JNB P3.6,FS
SS:
	MOV     COUNT,#LO             ;1ms * 250
	AJMP SSS
	//MOV     COUNT+1,#HIGH LO
FS:
	MOV     COUNT,#HI             ;1ms * 46
	//MOV     COUNT+1,#HIGH HI
SSS:

	CALL STEP
	JNB P3.7,REV
	INC TIK
	AJMP SSRR
REV:
	DEC TIK
		//INC TIK

SSRR:
	//MOV P0,TIK
	DEC R0
	CJNE R0,#0,SKIP
	MOV R0,#CIRC
	CALL DOIT
	
	
    //CPL     TEST_LED                    ;work LED flash
SKIP:
    CLR     C
    MOV     A,COUNT                     ;count--
    DEC A
    MOV     COUNT,A
	POP     PSW
    POP     ACC
    RETI

;-----------------------------------------------
STEP:
	ANL TIK,#03H
	MOV A,TIK
	CJNE A,#00H,AA1
	AJMP T10
AA1:CJNE A,#01H,AA2
	AJMP T11
AA2:CJNE A,#02H,AA3
	AJMP T01
AA3:
	AJMP T00
	
	

DOIT:
	INC R5
	CJNE R5, #10, DISPLAY
	
	MOV R5, #00H
	INC R6
	CJNE R6, #10, DISPLAY
	
	MOV R6, #00H
	INC R7
	CJNE R7, #10, DISPLAY
	
	MOV R5, #00H
	MOV R6, #00H
	MOV R7, #00H
DISPLAY:
	ACALL LEDD
	RET
	
LEDD:

	MOV A, R5
	MOVC A, @A+DPTR
	
	ACALL DISPLAYBYREG
	
	MOV A, R6
	MOVC A, @A+DPTR
	
	ACALL DISPLAYBYREG
	
	MOV A, R7
	MOVC A, @A+DPTR
	
	ACALL DISPLAYBYREG

	RET	
DISPLAYBYREG:
	
	MOV R1, #08H
	
SHIFTOUT:
	
	RLC A
	;SHIFT REGISTER
	MOV P4.5, C	;BIT DATA
	CLR P4.4
	SETB P4.4	;CLOCK
	
	DJNZ R1, SHIFTOUT
	
	RET
	
T01:
	//SETB P0.1
	CLR P3.2
	SETB P1.0
	SETB P1.1
	SETB P1.4
	RET	
T11:
	//SETB P0.2
	SETB P3.2
	SETB P1.0
	SETB P1.1
	SETB P1.4
	RET
T10:
	//SETB P0.3
	SETB P3.2
	CLR P1.0
	SETB P1.1
	SETB P1.4
	RET	
T00:
	//SETB P0.4
	CLR P3.2
	CLR P1.0
	SETB P1.1
	SETB P1.4
	RET	
	
    END
```
