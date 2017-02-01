;AREA FIR_Filter, CODE, READONLY
	
	;EXPORT FIR_asm
	; r0, input, r1, output, r2, size, r3, inputcoeffs
	
	; r5, multiplications will accumulate here
	
;FIR_asm

	;PUSH {lr}
	;VLDR.F32 s6, [r3,#0] 
	;VLDR.F32 s7, [r0,#0] 
	;VFMA.F32 s5, s6, s7
	
	;VLDR.F32 s6, [r3,#4] 
	;VLDR.F32 s7, [r0,#4] 
	;VFMA.F32 s5, s6, s7
	
	;VLDR.F32 s6, [r3,#8] 
	;VLDR.F32 s7, [r0,#8] 
	;VFMA.F32 s5, s6, s7

	;VLDR.F32 s6, [r3,#12] 
	;VLDR.F32 s7, [r0,#12] 
	;VFMA.F32 s5, s6, s7
	
	;VLDR.F32 s6, [r3,#16] 
	;VLDR.F32 s7, [r0,#16] 
	;VFMA.F32 s5, s6, s7
	
	;VSTR.F32 s5, [s1{,#0}]
	;POP {lr}
	
	;mov r8, #0
	

	;bl loadnext
	;bl loopA


;loadnext
	;ldr r4, [r0,r8]
	;ldr r5, [r3,r8]
	;ldr r7, [r1,r8]
	;mov PC, r14
	
;loopA
	;mul r7, r4, r5
	;cmp r8, #0
	;beq first
	;bne other
	
;first
	;str r7, [r1,r8]
	;add r8, r8, #4
	;sub r2, r2, #1
	;bl loadnext
	;bl loopA
;other
	;sub r2, r2, #1
	;sub r8, r8, #4
	;ldr r4, [r1,r8]
	;add r7, r7, r4
	;add r8, r8, #4
	;str r7, [r1,r8]
	;add r8, r8, #4
	;cmp r2, #0
	;beq terminate
	;bl loadnext
	;bl loopA
	
;terminate
	;POP {lr}
	
;END