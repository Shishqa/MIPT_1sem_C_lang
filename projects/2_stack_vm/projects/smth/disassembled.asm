	JMP label_0 
label_0:
	INIT_SCR 
	CLR 
	WIDTH 
	PUSH 2 
	DIV 
	POP ah 
	HEIGHT 
	PUSH 2 
	DIV 
	POP bh 
	PUSH ah 
	PUSH bh 
	ADDCH 219 
	MOV -1 dh 
	REF 
	JMP label_1 
label_1:
	GETCH 
	POP ax 
	PUSH ax 
	PUSH -1 
	JE label_2 
	PUSH ax 
	PUSH 119 
	JE label_3 
	PUSH ax 
	PUSH 97 
	JE label_4 
	PUSH ax 
	PUSH 100 
	JE label_5 
	PUSH ax 
	PUSH 93 
	JE label_6 
label_2:
	CALL label_7 
label_9:
	CALL label_8 
	JMP label_1 
label_6:
	CLR 
	END_WIN 
	END 
label_4:
	PUSH ah 
	PUSH bh 
	ADDCH 32 
	PUSH ah 
	PUSH 1 
	SUB 
	POP ah 
	PUSH ah 
	PUSH bh 
	ADDCH 219 
	REF 
	JMP label_2 
label_5:
	PUSH ah 
	PUSH bh 
	ADDCH 32 
	PUSH ah 
	PUSH 1 
	ADD 
	POP ah 
	PUSH ah 
	PUSH bh 
	ADDCH 219 
	REF 
	JMP label_2 
label_3:
	MOV 100 ch 
	JMP label_9 
label_7:
	PUSH ch 
	PUSH dh 
	ADD 
	POP ch 
	RET 
label_8:
	PUSH ah 
	PUSH bh 
	ADDCH 32 
	PUSH bh 
	PUSH ch 
	ADD 
	POP bh 
	PUSH bh 
	PUSH 3 
	JBE label_10 
	JMP label_11 
label_10:
	MOV 4 bh 
	MOV 0 ch 
label_11:
	PUSH ah 
	PUSH bh 
	ADDCH 219 
	REF 
	RET 

