; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "Fib"
func_Fib:
;	Fib_var_n (arg_1)
	POP [ex+1]
;	"Fib" body:

	if_0:
;	if_0_condition
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	2 	; NUM
	JB case_0_positive
	JMP case_0_negative
	case_0_positive:
		PUSH	1
		JMP	case_0_continue
	case_0_negative:
		PUSH	0
	case_0_continue:
		PUSH	0
		JNE	if_0_positive
		JMP	if_0_negative
	if_0_positive:

		PUSH	[ex+1] 	; LOCAL VAR
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET

		JMP	if_0_end
	if_0_negative:


	if_0_end:
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	1 	; NUM
		SUB
	POP	[ex+3]
		PUSH	[ex+3] 	; LOCAL VAR
	MOV	ex+50	ex
	CALL	func_Fib
	POP	[ex+2]
		PUSH	[ex+1] 	; LOCAL VAR
		PUSH	2 	; NUM
		SUB
	POP	[ex+5]
		PUSH	[ex+5] 	; LOCAL VAR
	MOV	ex+50	ex
	CALL	func_Fib
	POP	[ex+4]
		PUSH	[ex+2] 	; LOCAL VAR
		PUSH	[ex+4] 	; LOCAL VAR
		ADD
	POP	[ex+6]
		PUSH	[ex+6] 	; LOCAL VAR
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################

;#####################################################################
;	function "main"
func_main:
;	"main" body:

		PUSH	1 	; NUM
	POP	[ex+1]
		PUSH	18 	; NUM
	POP	[ex+2]
		PUSH	[ex+2] 	; LOCAL VAR
		PUSH	1 	; NUM
		ADD
	POP	[ex+3]
		PUSH	0 	; NUM
	POP	[ex+4]
		PUSH	0 	; NUM
	POP	[ex+5]
cycle_0:
		PUSH	[ex+4] 	; LOCAL VAR
		PUSH	[ex+3] 	; LOCAL VAR
	JB case_1_positive
	JMP case_1_negative
	case_1_positive:
		PUSH	1
		JMP	case_1_continue
	case_1_negative:
		PUSH	0
	case_1_continue:
	PUSH	0
	JE cycle_0_stop
		PUSH	[ex+4] 	; LOCAL VAR
	MOV	ex+50	ex
	CALL	func_Fib
	POP	[ex+5]
		PUSH	[ex+5] 	; LOCAL VAR
	OUT
		PUSH	[ex+4] 	; LOCAL VAR
		PUSH	1 	; NUM
		ADD
	POP	[ex+4]
	JMP cycle_0
cycle_0_stop:
		PUSH	0 	; NUM
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################

