	JMP start
	
start:
	PUSH 1
	POP ax
	IN
	POP bx
cycle:
	PUSH ax
	PUSH bx
	JA stop
	PUSH ax
	POP ah
	CALL square ; возвести в квадрат ax
	PUSH ah
	OUT
	OUTC 10
	PUSH ax
	PUSH 1
	ADD
	POP ax
	JMP cycle 
stop: 
	END
	
;--------------------------------------------------------------------------------
square:
	PUSH ah
	PUSH ah
	MUL
	POP ah
	RET
;--------------------------------------------------------------------------------

