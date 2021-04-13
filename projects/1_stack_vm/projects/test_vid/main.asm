	INIT_SCR
	CLR
	JMP screen_
	
screen_:
	MOV	0	ax
	
	WIDTH
	PUSH	2
	DIV
	POP		al
	
	HEIGHT
	PUSH	2
	DIV
	POP		bl
	
	cycle_:
		MOV	ax  bh
		CALL   set_mem
		REF
		
		PUSH	ax
		PUSH	1
		ADD
		POP		ax
		
		PUSH	ax
		HEIGHT
		PUSH	2
		DIV
		JAE		stop
		
		JMP		cycle_

	stop:
		GETCH
		POP	ax
		
		PUSH	ax
		PUSH	93
		JE		end_
		
		JMP		stop
		
end_:
	END_WIN
	END
	
;-----------------------------------------------------	
set_mem:	
	MOV	0 ch
	
	cyc:
		PUSH	ch
	
		PUSH al+1
		PUSH bl+1
		MUL
		
		JAE cyc_stop
		
		PUSH	ch
		PUSH	al+1
		MOD
		PUSH	al
		SUB
		PUSH	ch
		PUSH	al+1
		MOD
		PUSH	al
		SUB
		PUSH	10000
		MUL
		MUL
		PUSH	4
		DIV
		
		PUSH	ch
		PUSH	al+1
		DIV
		PUSH	bl
		SUB
		PUSH	ch
		PUSH	al+1
		DIV
		PUSH	bl
		SUB
		PUSH	10000
		MUL
		MUL
		
		ADD
		
		SQRT 4
		
		POP		dh
		
		PUSH	ch
		PUSH	al+1
		DIV
		WIDTH	
		MUL
		PUSH	ch
		PUSH	al+1
		MOD
		ADD
		POP		ah
		
		PUSH   dh
		PUSH	bh
		PUSH	10000
		MUL
		JAE		set_0
		
		PUSH	dh
		PUSH	bh
		PUSH	2
		SUB
		PUSH	10000
		MUL
		JB		set_0
		
		JMP		set_1
		
		
	set_1:
		MOV	2 [ah+100000]
		PUSH	ah
		WIDTH
		DIV
		WIDTH
		MUL
		WIDTH
		PUSH	ah
		WIDTH
		MOD
		SUB
		ADD
		POP	ah
		MOV	2 [ah+100000]

		JMP		next
	
	set_0:
		MOV	1 [ah+100000]
		WIDTH
		PUSH	ah
		WIDTH
		MOD
		SUB
		POP	ah
		MOV	1 [ah+100000]
		
		JMP		next
		
	next:
		PUSH	ch
		PUSH	1
		ADD
		POP		ch
		JMP		cyc
		
	cyc_stop:
		RET
		
		