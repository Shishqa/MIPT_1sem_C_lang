	JMP init_
	
init_:
	INIT_SCR
	CLR
	WIDTH
	PUSH 		2
	DIV
	POP			ah
	HEIGHT
	PUSH		2
	DIV
	POP			bh
	
	PUSH		ah
	PUSH		bh
	ADDCH		219
	
	MOV			-1 dh
	
	REF
	JMP	screen_
	
	
screen_:
	GETCH
	POP			ax	
	
	PUSH		ax
	PUSH		-1
	JE				gravity_
	
	PUSH		ax
	PUSH		119
	JE				mov_up
	
	PUSH		ax
	PUSH		97
	JE				mov_left
	
	PUSH		ax
	PUSH		100
	JE				mov_right
	
	PUSH		ax
	PUSH		93
	JE				end_screen_
	
gravity_:
	CALL 		gravity
	
move_:
	CALL			move
	
	JMP			screen_

	
end_screen_:
	CLR
	END_WIN
	END 
	

mov_left:
	PUSH		ah
	PUSH		bh
	ADDCH		32
	PUSH		ah
	PUSH		1
	SUB
	POP			ah
	PUSH		ah
	PUSH		bh
	ADDCH		219
	REF
	JMP			gravity_
	
	
mov_right:
	PUSH		ah
	PUSH		bh
	ADDCH		32
	PUSH		ah
	PUSH		1
	ADD
	POP			ah
	PUSH		ah
	PUSH		bh
	ADDCH		219
	REF
	JMP			gravity_
	
	
mov_up:
	MOV			100 ch
	JMP			move_
	
	
gravity:
	PUSH		ch
	PUSH		dh
	ADD			
	POP			ch
	RET
	
move:
	PUSH		ah
	PUSH		bh
	ADDCH		32
	PUSH		bh
	PUSH		ch
	ADD
	POP			bh
	
	PUSH		bh
	PUSH		3
	JBE			coll
	JMP			draw
	
coll:
	MOV			4 bh
	MOV			0 ch
	
draw:
	PUSH		ah
	PUSH		bh
	ADDCH		219
	REF
	RET