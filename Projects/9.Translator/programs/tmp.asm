; ##      THIS FILE IS GENERATED AUTOMATICALLY      ##
; ## CHANGE THE ORIGIN IN ORDER TO CHANGE THIS FILE ##
; ORIGIN :: programs/get_chars/main.plan

	CALL	func_main
	MOV		100	ex
	END

;#####################################################################
;	function "main"
func_main:
;	"main" body:

	IN
	POP	[ex+1]
cycle_0:
		PUSH	[ex+1]
		PUSH	0
	JNE case_0_positive
	JMP case_0_negative
	case_0_positive:
		PUSH	1
		JMP	case_0_continue
	case_0_negative:
		PUSH	0
		JMP	case_0_continue
	case_0_continue:
	PUSH	0
	JE cycle_0_stop
; pushing argument before call putc
		PUSH	[ex+1]
	OUTC
; pushing argument before call putc
		PUSH	10
	OUTC
	IN
	POP	[ex+1]
	JMP cycle_0
cycle_0_stop:
	PUSH	ex
	PUSH	50
	SUB
	POP	ex
	RET
;#####################################################################


