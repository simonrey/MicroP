	;AREA FIR_Filter, CODE, READONLY
	
	;EXPORT FIR_asm
	
;FIR_asm
	;;r0 input array
	;;r1 output array
	;;r2 size
	;;r3 coefficients array
	
	;;r4 input
	;;r5 coefficient
	;;r6 storage counter
	;;r7 output
	;PUSH {lr}
	;sub r2, r2, #4
	;mov r8, #0
	;mov r6, #0
	;ldr r7, [r1,r8]
	;bl loadNextItem


;loadNextItem
	;ldr r5, [r3,r8]
	;add r8, r8, r6
	;ldr r4, [r0,r8]
	;sub r8, r8, r6
	;bl loopA
	
;loopA
	;mla r7, r4, r5, r7
	;cmp r8, #16
	;beq anew
	;add r8, r8, #4
	;bl loadNextItem
	
;anew
	;str r7, [r1,r6]
	;add r6, r6, #4
	;sub r2, r2, #1
	;cmp r2, #0
	;beq terminate
	;mov r8, #0
	;mov r7, #0
	;bl loadNextItem
	
;terminate
	;POP{LR}
	;END