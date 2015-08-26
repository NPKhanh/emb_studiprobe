	AREA GetChar_UART, CODE
	INCLUDE	LPC2368_asm.inc
	EXPORT 	UART_GetChar
	;Assumption R0 = UART_PortNum (0 or 1 for EVB MCB2300), R0 = Char to be read
	
UART_GetChar
    STMFD   SP!,{R4,R5, LR}  ;Retten Der Daten
    ANDS 	R0, R0, #0x01  ; Check if UART Port 0 or Port 1 selected
	BEQ 	UART_Port0 ;Port 0 oder Port1 ausgewählt
UART_Port1	
	LDR		R3,=U1LSR     ; UART Port 1 selected
	;LDR		R4,=U1THR
	LDR 	R4,=U1RBR
Wait_TX1	
	LDR 	R5, [R3] ; Content of U1LSR loaded in R3
	ANDS	R5, R5, #2_00000001 ; Test bit 5 of U1LSR -> RDR sind Daten vorhanden?
	BEQ 	kill 
	LDR		R0, [R4]
	B UART_Tx_Done
kill
	LDR R0,=0xFFFFFFFF

UART_Port0
	LDR		R3,=U0LSR    ;UART Port 0 selected
	;LDR		R4,=U0THR
	LDR 	R4,=U0RBR
Wait_TX0	
	LDR 	R5, [R3] ; Content of U0LSR loaded in R3
	ANDS	R5, R5, #2_00000001 ; Test bit 1 of U0LSR
	BEQ 	kill_2
	LDR		R0, [R4]
	B UART_Tx_Done
kill_2
	LDR R0,=0xFFFFFFFF
	
UART_Tx_Done
   LDMFD   SP!,{R4,R5, PC}  ;wiederherstellen der daten
   ;MOV 		R15, R14  ;Return from branch with link 	
	
   END  
    
