; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
; ORIGIN :: programs/fibonacci/main.plan

	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "Fib"
func_Fib:
;	Fib_var_num (arg_1)
	POP [ex+1]
;	"Fib" body:

	if_0:
;	if_0_condition
		PUSH	[ex+1]
		PUSH	1
	JBE case_0_positive
	JMP case_0_negative
	case_0_positive:
		PUSH	1
		JMP	case_0_continue
	case_0_negative:
		PUSH	0
		JMP	case_0_continue
	case_0_continue:
		PUSH	0
		JNE	if_0_positive
		JMP	if_0_negative
	if_0_positive:

; pushing argument before call return
		PUSH	[ex+1]
	MOV	ex+0	ex
	CALL	func_(null)

		JMP	if_0_end
	if_0_negative:


	if_0_end:
; pushing argument before call return
; pushing argument before call Fib
		PUSH	[ex+1]
		PUSH	1
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
; pushing argument before call Fib
		PUSH	[ex+1]
		PUSH	2
		SUB
	MOV	ex+50	ex
	CALL	func_Fib
		ADD
	MOV	ex+0	ex
	CALL	func_(null)
;#####################################################################


;#####################################################################
;	function "main"
func_main:
;	"main" body:

	MOV	ex+0	ex
	CALL	func_(null)
	POP	[ex+1]
	MOV	ex+0	ex
	CALL	func_(null)
	POP	[ex+2]
	if_1:
;	if_1_condition
		PUSH	[ex+1]
		PUSH	1
	JE case_1_positive
	JMP case_1_negative
	case_1_positive:
		PUSH	1
		JMP	case_1_continue
	case_1_negative:
		PUSH	0
		JMP	case_1_continue
	case_1_continue:
		PUSH	0
		JNE	if_1_positive
		JMP	if_1_negative
	if_1_positive:

		PUSH	0
	POP	[ex+3]
cycle_0:
		PUSH	[ex+3]
		PUSH	[ex+2]
	JBE case_2_positive
	JMP case_2_negative
	case_2_positive:
		PUSH	1
		JMP	case_2_continue
	case_2_negative:
		PUSH	0
		JMP	case_2_continue
	case_2_continue:
	PUSH	0
	JE cycle_0_stop
; pushing argument before call print
; pushing argument before call Fib
		PUSH	[ex+3]
	MOV	ex+50	ex
	CALL	func_Fib
	MOV	ex+0	ex
	CALL	func_(null)
		PUSH	[ex+3]
		PUSH	1
		ADD
	POP	[ex+3]
	JMP cycle_0
cycle_0_stop:

		JMP	if_1_end
	if_1_negative:

; pushing argument before call print
; pushing argument before call Fib
		PUSH	[ex+2]
	MOV	ex+50	ex
	CALL	func_Fib
	MOV	ex+0	ex
	CALL	func_(null)

	if_0_end:
; pushing argument before call return
		PUSH	0
	MOV	ex+0	ex
	CALL	func_(null)
;#####################################################################


