	INIT_SCR 
	CLR 
	JMP label_0 
label_0:
	WIDTH 
	POP al 
	HEIGHT 
	POP bl 
	PUSH bl 
	PUSH 2 
	DIV 
	POP cl 
	PUSH al 
	PUSH 2 
	DIV 
	POP ch 
	PUSH bl 
	PUSH 2 
	DIV 
	POP dh 
	MOV 0 bx 
label_6:
	MOV 1 ah 
label_4:
	PUSH ah 
	PUSH cl 
	JA label_1 
	CALL label_2 
	REF 
	GETCH 
	PUSH 1 
	JE label_3 
	PUSH ah 
	PUSH 1 
	ADD 
	POP ah 
	JMP label_4 
label_1:
	PUSH ah 
	PUSH 1 
	JB label_5 
	CALL label_2 
	REF 
	GETCH 
	PUSH 1 
	JE label_3 
	PUSH ah 
	PUSH 1 
	SUB 
	POP ah 
	JMP label_1 
label_5:
	JMP label_6 
label_3:
	END_WIN 
	END 
label_2:
	MOV 0 bh 
label_11:
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	PUSH dh 
	PUSH 1 
	ADD 
	MUL 
	JA label_7 
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	MOD 
	PUSH ch 
	SUB 
	PUSH 2 
	DIV 
	PUSH 10 
	MUL 
	POP cx 
	PUSH cx 
	PUSH cx 
	MUL 
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	DIV 
	PUSH dh 
	SUB 
	PUSH 10 
	MUL 
	POP cx 
	PUSH cx 
	PUSH cx 
	MUL 
	ADD 
	SQRT 2 
	PUSH 100 
	DIV 
	POP cx 
	PUSH cx 
	PUSH ah 
	JAE label_8 
	PUSH cx 
	PUSH ah 
	PUSH 2 
	SUB 
	JBE label_8 
	JMP label_9 
label_9:
	MOV 1 [bh100000] 
	JMP label_10 
label_8:
	MOV 0 [bh100000] 
	JMP label_10 
label_10:
	PUSH bh 
	PUSH 1 
	ADD 
	POP bh 
	JMP label_11 
label_7:
	RET 
