	JMP input
	
input:	
	OUTC	65
	OUTC	58
	OUTC	32
	
	IN
	POP 	dh
	PUSH	dh
	PUSH	0
	JB		exit
	PUSH 	10
	PUSH	dh
	POW 	
	POP		ch
	IN
	PUSH	ch
	MUL
	POP 	ax
	IN 
	PUSH	ch
	MUL
	POP 	bx
	IN
	PUSH	ch
	MUL
	POP 	cx
	CALL 	solve_square_equation
exit:
	END
	
solve_square_equation:
	PUSH 	ax
	PUSH 	0
	JE 		solve_linear_equation
	
	PUSH 	bx
	PUSH 	bx
	MUL
	PUSH	ch
	DIV
	
	PUSH 	4
	PUSH 	ax
	PUSH 	cx
	MUL
	PUSH	ch
	DIV
	MUL
	SUB
	
	POP 	dx
	
	PUSH 	dx
	PUSH 	0
	JB 		zero_roots
	
	PUSH 	dx
	PUSH 	0
	JE 		get_one_root
	
	JMP 	get_two_roots
	
get_one_root:
	PUSH 	bx
	PUSH 	-1
	MUL
	PUSH	ch
	MUL
	PUSH 	2
	PUSH 	ax
	MUL
	DIV
	PUSH 	dx
	PUSH 	0
	JE 		one_root
	JMP 	get_two_roots
	
get_two_roots:
	PUSH 	bx
	PUSH 	-1
	MUL
	PUSH	ch
	MUL
	PUSH 	2
	PUSH 	ax
	MUL
	DIV
	PUSH 	dx
	SQRT 	dh
	PUSH	ch
	MUL
	PUSH 	2
	PUSH 	ax
	MUL
	DIV
	SUB
	PUSH 	bx
	PUSH 	-1
	MUL
	PUSH	ch
	MUL
	PUSH 	2
	PUSH 	ax
	MUL
	DIV
	PUSH 	dx
	SQRT 	dh
	PUSH	ch
	MUL
	PUSH 	2
	PUSH 	ax
	MUL
	DIV
	ADD
	JMP 	two_roots
	
solve_linear_equation:
	PUSH 	bx
	PUSH 	0 
	JE 		solve_equality
	PUSH 	cx
	PUSH 	-1
	PUSH	ch
	MUL
	MUL
	PUSH 	bx
	DIV
	JMP 	one_root
	
solve_equality:
	PUSH 	cx
	PUSH 	0
	JE 		infinite_roots
	JMP 	zero_roots
	
infinite_roots:
	OUTC 	73
	OUTC 	10
	RET
	
zero_roots:
	PUSH 	0
	OUT 
	OUTC 	10
	RET

one_root:
	PUSH 	1
	OUT 
	OUTC 	32
	OUTF	dh 
	OUTC 	10
	RET

two_roots:
	PUSH 	2
	OUT 
	OUTC 	32
	OUTF 	dh
	OUTC 	32
	OUTF	dh 
	OUTC 	10
	RET
