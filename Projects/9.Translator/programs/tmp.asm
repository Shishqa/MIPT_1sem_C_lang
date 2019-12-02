; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
; ORIGIN :: programs/fibonacci/main.plan

	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "Fib"
func_Fib:
;	Fib_var_num (arg_0)
	POP [ex+0]
;	"Fib" body:

	if_0:
;	case_0_condition
		PUSH	[ex+0]
		PUSH	1
	JBE case_1_positive
	JMP case_1_negative
	case_1_positive:
		PUSH	1
		JMP	case_1_continue
	case_1_negative:
		PUSH	0
		JMP	case_1_continue
	case_1_continue:
		PUSH	0
		JNE	case_0_operators
		JMP	case_0_next
	case_0_operators:

; pushing argument before call return
		PUSH	[ex+0]
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET

		JMP	if_0_end
	case_0_next:

	if_0_end:

; pushing argument before call return
; pushing argument before call Fib
		PUSH	[ex+0]
		PUSH	1
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
; pushing argument before call Fib
		PUSH	[ex+0]
		PUSH	2
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
		ADD
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

; pushing argument before call putc
		PUSH	77
	OUTC
; pushing argument before call putc
		PUSH	58
	OUTC
; pushing argument before call putc
		PUSH	32
	OUTC
	IN
	POP	[ex+0]
; pushing argument before call putc
		PUSH	78
	OUTC
; pushing argument before call putc
		PUSH	58
	OUTC
; pushing argument before call putc
		PUSH	32
	OUTC
	IN
	POP	[ex+1]
	if_1:
;	case_2_condition
		PUSH	[ex+0]
		PUSH	1
	JE case_3_positive
	JMP case_3_negative
	case_3_positive:
		PUSH	1
		JMP	case_3_continue
	case_3_negative:
		PUSH	0
		JMP	case_3_continue
	case_3_continue:
		PUSH	0
		JNE	case_2_operators
		JMP	case_2_next
	case_2_operators:

		PUSH	0
	POP	[ex+2]
cycle_0:
		PUSH	[ex+2]
		PUSH	[ex+1]
	JBE case_4_positive
	JMP case_4_negative
	case_4_positive:
		PUSH	1
		JMP	case_4_continue
	case_4_negative:
		PUSH	0
		JMP	case_4_continue
	case_4_continue:
	PUSH	0
	JE cycle_0_stop
; pushing argument before call print
; pushing argument before call Fib
		PUSH	[ex+2]
	MOV	ex+50	ex
	CALL	func_Fib
	OUT
; pushing argument before call putc
		PUSH	32
	OUTC
		PUSH	[ex+2]
		PUSH	1
		ADD
	POP	[ex+2]
	JMP cycle_0
cycle_0_stop:

		JMP	if_1_end
	case_2_next:

; pushing argument before call print
; pushing argument before call Fib
		PUSH	[ex+1]
	MOV	ex+50	ex
	CALL	func_Fib
	OUT
	if_1_end:

; pushing argument before call return
		PUSH	0
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################


