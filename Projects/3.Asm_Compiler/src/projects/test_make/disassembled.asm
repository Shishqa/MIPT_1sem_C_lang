	JMP label_0 
label_0:
	PUSH 1 
	POP ax 
	IN 
	POP bx 
label_3:
	PUSH ax 
	PUSH bx 
	JA label_1 
	PUSH ax 
	POP ah 
	CALL label_2 
	PUSH ah 
	OUT 
	OUTC 10 
	PUSH ax 
	PUSH 1 
	ADD 
	POP ax 
	JMP label_3 
label_1:
	END 
label_2:
	PUSH ah 
	PUSH ah 
	MUL 
	POP ah 
	RET 
