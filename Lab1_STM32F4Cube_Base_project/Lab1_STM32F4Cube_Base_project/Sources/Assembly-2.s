	;AREA FIR_Filter, CODE, READONLY
	
	;EXPORT FIR_asm
	
;FIR_asm
	;;r0 input array
	;;r1 output array
	;;r2 size
	;;r3 coefficients array
	
	;;r4 input
	;;r5 coefficient
	;;r7 output
	
	;mov r8, #0
	;mov r6, #0
	;ldr r7, [r1,r8]
	;bl loadnext


;loadnext
	;ldr r4, [r0,r8]
	;ldr r5, [r3,r8]
	;bl loopA
	
;loopA
	;mla r7, r4, r5, r7
	;cmp r8, #16
	;beq anew
	;add r8, r8, #4
	;bl loadnext
	
;anew
	;str r7, [r1,r6]
	;add r6, r6, #4
	;sub r2, r2, #1
	;cmp r2, #0
	;beq terminate
	;lsl r0,r0,#31
	;mov r8, #0
	;mov r7, #0
	;bl loadnext
	
;terminate
	
	;END