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
label_7:
	MOV 1 ah 
label_5:
	PUSH ah 
	PUSH cl 
	JA label_1 
	CALL label_2 
	CALL label_3 
	GETCH 
	PUSH 1 
	JE label_4 
	PUSH ah 
	PUSH 1 
	ADD 
	POP ah 
	JMP label_5 
label_1:
	PUSH ah 
	PUSH 1 
	JB label_6 
	CALL label_2 
	CALL label_3 
	GETCH 
	PUSH 1 
	JE label_4 
	PUSH ah 
	PUSH 1 
	SUB 
	POP ah 
	JMP label_1 
label_6:
	JMP label_7 
label_4:
	END_WIN 
	END 
label_2:
	MOV 0 bh 
label_12:
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	PUSH dh 
	PUSH 1 
	ADD 
	MUL 
	JA label_8 
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
	JAE label_9 
	PUSH cx 
	PUSH ah 
	PUSH 2 
	SUB 
	JBE label_9 
	JMP label_10 
label_10:
	MOV 1 [bh] 
	JMP label_11 
label_9:
	MOV 0 [bh] 
	JMP label_11 
label_11:
	PUSH bh 
	PUSH 1 
	ADD 
	POP bh 
	JMP label_12 
label_8:
	RET 
label_3:
	MOV 0 bh 
label_17:
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	PUSH dh 
	PUSH 1 
	ADD 
	MUL 
	JA label_13 
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	MOD 
	POP cx 
	PUSH bh 
	PUSH ch 
	PUSH 1 
	ADD 
	DIV 
	POP dx 
	PUSH [bh] 
	PUSH 0 
	JE label_14 
	JMP label_15 
label_14:
	PUSH cx 
	PUSH dx 
	ADDCH 32 
	WIDTH 
	PUSH cx 
	SUB 
	PUSH dx 
	ADDCH 32 
	PUSH cx 
	HEIGHT 
	PUSH dx 
	SUB 
	ADDCH 32 
	WIDTH 
	PUSH cx 
	SUB 
	HEIGHT 
	PUSH dx 
	SUB 
	ADDCH 32 
	JMP label_16 
label_15:
	PUSH cx 
	PUSH dx 
	ADDCH 219 
	WIDTH 
	PUSH cx 
	SUB 
	PUSH dx 
	ADDCH 219 
	PUSH cx 
	HEIGHT 
	PUSH dx 
	SUB 
	ADDCH 219 
	WIDTH 
	PUSH cx 
	SUB 
	HEIGHT 
	PUSH dx 
	SUB 
	ADDCH 219 
	JMP label_16 
label_16:
	PUSH bh 
	PUSH 1 
	ADD 
	POP bh 
	JMP label_17 
label_13:
	REF 
	RET 
