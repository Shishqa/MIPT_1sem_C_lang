	INIT_SCR 
	CLR 
	JMP label_0 
label_0:
	MOV 0 ax 
	WIDTH 
	PUSH 2 
	DIV 
	POP al 
	HEIGHT 
	PUSH 2 
	DIV 
	POP bl 
label_3:
	MOV ax bh 
	CALL label_1 
	REF 
	PUSH ax 
	PUSH 1 
	ADD 
	POP ax 
	PUSH ax 
	HEIGHT 
	PUSH 2 
	DIV 
	JAE label_2 
	JMP label_3 
label_2:
	GETCH 
	POP ax 
	PUSH ax 
	PUSH 93 
	JE label_4 
	JMP label_2 
label_4:
	END_WIN 
	END 
label_1:
	MOV 0 ch 
label_9:
	PUSH ch 
	PUSH al1 
	PUSH bl1 
	MUL 
	JAE label_5 
	PUSH ch 
	PUSH al1 
	MOD 
	PUSH al 
	SUB 
	PUSH ch 
	PUSH al1 
	MOD 
	PUSH al 
	SUB 
	PUSH 10000 
	MUL 
	MUL 
	PUSH 4 
	DIV 
	PUSH ch 
	PUSH al1 
	DIV 
	PUSH bl 
	SUB 
	PUSH ch 
	PUSH al1 
	DIV 
	PUSH bl 
	SUB 
	PUSH 10000 
	MUL 
	MUL 
	ADD 
	SQRT 4 
	POP dh 
	PUSH ch 
	PUSH al1 
	DIV 
	WIDTH 
	MUL 
	PUSH ch 
	PUSH al1 
	MOD 
	ADD 
	POP ah 
	PUSH dh 
	PUSH bh 
	PUSH 10000 
	MUL 
	JAE label_6 
	PUSH dh 
	PUSH bh 
	PUSH 2 
	SUB 
	PUSH 10000 
	MUL 
	JB label_6 
	JMP label_7 
label_7:
	MOV 2 [ah100000] 
	PUSH ah 
	WIDTH 
	DIV 
	WIDTH 
	MUL 
	WIDTH 
	PUSH ah 
	WIDTH 
	MOD 
	SUB 
	ADD 
	POP ah 
	MOV 2 [ah100000] 
	JMP label_8 
label_6:
	MOV 1 [ah100000] 
	WIDTH 
	PUSH ah 
	WIDTH 
	MOD 
	SUB 
	POP ah 
	MOV 1 [ah100000] 
	JMP label_8 
label_8:
	PUSH ch 
	PUSH 1 
	ADD 
	POP ch 
	JMP label_9 
label_5:
	RET 
