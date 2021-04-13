	INIT_SCR
	CLR
	JMP screen_
	
screen_:
	WIDTH
	POP			al	;COLS
	HEIGHT
	POP			bl	;LINES
	; --
	PUSH		bl
	PUSH		2
	DIV
	POP			cl	;max radius
	; -- 
	PUSH		al
	PUSH		2
	DIV
	POP			ch  	;center x
	; -- 
	PUSH		bl
	PUSH		2
	DIV
	POP			dh  	;center y
	; --	
	MOV		0 bx
	
	
circulate:	
	MOV		1 ah
	
go_up:
	PUSH		ah
	PUSH		cl
	JA			go_down
	CALL		set_mem
	CALL		present
	GETCH
	PUSH		1
	JE			end_screen_
	PUSH		ah
	PUSH		1
	ADD
	POP			ah
	JMP			go_up
	
go_down:
	PUSH		ah
	PUSH		1
	JB			cir_next
	CALL		set_mem
	CALL		present
	GETCH
	PUSH		1
	JE			end_screen_
	PUSH		ah
	PUSH		1
	SUB
	POP			ah
	JMP			go_down
	
cir_next:
	JMP			circulate
	
	

	
	
end_screen_:
	END_WIN
	END
	
	
;-- ah - radius
set_mem:
	MOV	0 bh
	
	cycle:
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		PUSH	dh
		PUSH	1
		ADD
		MUL
		JA		stop
		; --
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		MOD
		PUSH	ch
		SUB
		PUSH	2
		DIV
		PUSH	10
		MUL
		POP		cx
		PUSH	cx
		PUSH	cx
		MUL
		; -
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		DIV
		PUSH	dh
		SUB
		PUSH	10
		MUL
		POP		cx
		PUSH	cx
		PUSH	cx
		MUL
		; -
		ADD
		SQRT	2
		PUSH	100
		DIV
		POP		cx
		; -- 
		PUSH	cx
		PUSH	ah
		JAE		clear
		PUSH	cx
		PUSH	ah
		PUSH	2
		SUB
		JBE		clear
		JMP		fill
		
		fill:
			MOV	1 [bh]
			JMP		next
		clear:
			MOV	0 [bh]
			JMP		next
			
	next:
		PUSH	bh
		PUSH	1
		ADD
		POP		bh
		JMP		cycle
	stop:
	
	RET
;--------------------
	
	
;--------------------
present:
	MOV	0 bh
	
	p_cycle:
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		PUSH	dh
		PUSH	1
		ADD
		MUL
		JA		p_stop
		; --
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		MOD
		POP		cx
		PUSH	bh
		PUSH	ch
		PUSH	1
		ADD
		DIV
		POP		dx
		
		PUSH	[bh]
		PUSH	0
		JE		empty
		JMP		dot
		
		empty:
			PUSH		cx
			PUSH		dx
			ADDCH 	32
			WIDTH
			PUSH		cx
			SUB
			PUSH		dx
			ADDCH 	32
			PUSH		cx
			HEIGHT
			PUSH		dx
			SUB
			ADDCH 	32
			WIDTH
			PUSH		cx
			SUB
			HEIGHT
			PUSH		dx
			SUB
			ADDCH 	32
			JMP			p_next
		dot:
			PUSH		cx
			PUSH		dx
			ADDCH 	219
			WIDTH
			PUSH		cx
			SUB
			PUSH		dx
			ADDCH 	219
			PUSH		cx
			HEIGHT
			PUSH		dx
			SUB
			ADDCH 	219
			WIDTH
			PUSH		cx
			SUB
			HEIGHT
			PUSH		dx
			SUB
			ADDCH 	219
			JMP			p_next
	
	p_next:
		PUSH 	bh
		PUSH	1
		ADD
		POP		bh
		JMP		p_cycle
		
	p_stop:
		REF
		RET
; ------------------------------------