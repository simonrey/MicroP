	AREA FIR_Filter, CODE, READONLY
	
	EXPORT FIR_asm
	 ;;r0, input, r1, output, r2, size, r3, inputcoeffs
	
FIR_asm						;called in C by "FIR_asm(float32_t * InputArray, float32_t* OutputArray,int blockSize, float32_t* coeffs)"
	VLDM.F32 r3, {s0, s1, s2, s3, s4}	;loading all coeffs into registers at one time
	SUB r2, r2, #4				;set r2 to length minus the order, gives the number of loops needed to cover entire input array
	LSL r2, r2, #2				;multiply by 4 the number of loops needed, gives the amount the ouput pointer will be incremented by the end
	ADD r3, r1, r2				;add the number of bytes to be incremented to the address of current output pointer
	
	VLDM.F32 r0!, {s5, s6, s7, s8, s9}	;loading first five inputs all at one time
loop	

	VMUL.F32 s5, s0, s5			;multiplies the first coeff with its input, s5 can now store the output as its input is no longer needed 
	VMUL.F32 s10, s1, s6			;multiplies the second coeff with its input
	VADD.F32 s5, s10, s5			;sums all the input coeff products, using VMUL then VADD uses 2 clock cycles, VFMA takes three
	VMUL.F32 s10, s2, s7
	VADD.F32 s5, s10, s5
	VMUL.F32 s10, s3, s8
	VADD.F32 s5, s10, s5
	VMUL.F32 s10, s4, s9
	VADD.F32 s5, s10, s5			;all coeffs have been multiplies by their inputs and summed together in s5
	
	VSTR.F32 s5, [r1],#4 			;the sum is saved in the output array, its pointer is incremented
	
	CMP r1, r3				;compares the pointer value of the output array to its end condition
	beq endLoop				;if end condition is met the loop is broken, otherwise continues to next section 
					
	VLDR.F32 s5, [r0],#4			;loads the new input and increments the input pointer, done after the CMP as its not needed if loop ends

	VMUL.F32 s6, s0, s6			;inputs are all shifted down from last summation 
	VMUL.F32 s10, s1, s7			;this is much faster than using the mov instruction
	VADD.F32 s6, s10, s6
	VMUL.F32 s10, s2, s8
	VADD.F32 s6, s10, s6
	VMUL.F32 s10, s3, s9
	VADD.F32 s6, s10, s6
	VMUL.F32 s10, s4, s5	
	VADD.F32 s6, s10, s6	
	
	VSTR.F32 s6, [r1],#4 					
	
	CMP r1, r3							
	beq endLoop	
		
	VLDR.F32 s6, [r0],#4		

	VMUL.F32 s7, s0, s7					
	VMUL.F32 s10, s1, s8
	VADD.F32 s7, s10, s7	
	VMUL.F32 s10, s2, s9
	VADD.F32 s7, s10, s7
	VMUL.F32 s10, s3, s5
	VADD.F32 s7, s10, s7
	VMUL.F32 s10, s4, s6
	VADD.F32 s7, s10, s7

	
	VSTR.F32 s7, [r1],#4 				
	
	CMP r1, r3							
	beq endLoop	
		
	VLDR.F32 s7, [r0],#4				

	VMUL.F32 s8, s0, s8					 
	VMUL.F32 s10, s1, s9	
	VADD.F32 s8, s10, s8	
	VMUL.F32 s10, s2, s5
	VADD.F32 s8, s10, s8
	VMUL.F32 s10, s3, s6
	VADD.F32 s8, s10, s8
	VMUL.F32 s10, s4, s7	
	VADD.F32 s8, s10, s8
	
	VSTR.F32 s8, [r1],#4 							
	
	CMP r1, r3							
	beq endLoop	

	VLDR.F32 s8, [r0],#4	
	
	VMUL.F32 s9, s0, s9					
	VMUL.F32 s10, s1, s5
	VADD.F32 s9, s10, s9
	VMUL.F32 s10, s2, s6
	VADD.F32 s9, s10, s9
	VMUL.F32 s10, s3, s7
	VADD.F32 s9, s10, s9
	VMUL.F32 s10, s4, s8	
	VADD.F32 s9, s10, s9
	
	
	VSTR.F32 s9, [r1],#4 				
	VLDR.F32 s9, [r0],#4				;this is done before the compare unlike the others since the beginning of the loop is entered at the start
							;with this register already loaded with a fresh value
	
	CMP r1, r3					;compares the pointer value of the output array to its end condition
	bne loop					;if the end condition is not met then the loop restarts as all input registers have shifted through
							;otherwise continue to the end of this subroutine
endLoop
	bx lr						;return to calling function, only happens if the end condition is met

;another version thats slower but less code

	;MACRO
	;sumStoreCompare
	
	;;VMUL.F32 s5, s0, s5					;multiplies the first coeff with its input, s5 can now store the output as its input is no longer needed 
	;;VFMA.F32 s5, s1, s6					;multiplies the second with its input and sums
	;;VFMA.F32 s5, s2, s7
	;;VFMA.F32 s5, s3, s8
	;;VFMA.F32 s5, s4, s9				;all coeffs have been multiplies by their inputs and summed together in s5
	;VMUL.F32 s5, s0, s5					;multiplies the first coeff with its input, s5 can now store the output as its input is no longer needed 
	;VMUL.F32 s10, s1, s6
	;VADD.F32 s5, s10, s5
	;VMUL.F32 s10, s2, s7
	;VADD.F32 s5, s10, s5
	;VMUL.F32 s10, s3, s8
	;VADD.F32 s5, s10, s5
	;VMUL.F32 s10, s4, s9
	;VADD.F32 s5, s10, s5	
	
	;VSTR.F32 s5, [r1],#4 				;the sum is saved in the output array, its pointer is incremented
	;CMP r1, r3							;compares the pointer value of the output array to its end condition
	;bne loop							;if it does not meet the end condition loop
	;MEND
	
;FIR_asm									;called in C by "FIR_asm(float32_t * InputArray, float32_t* OutputArray,int blockSize, float32_t* coeffs)"
	;VLDM.F32 r3, {s0, s1, s2, s3, s4}	;loading all coeffs into registers at one time
	;SUB r2, r2, #4						;set r2 to length minus the order, gives the number of loops needed to cover entire input array
	;LSL r2, r2, #2						;multiply by 4 the number of loops needed, gives the amount the ouput pointer will be incremented by the end
	;ADD r3, r1, r2						;add the number of bytes to be incremented to the address of current output pointer
	
	;VLDM.F32 r0!, {s5, s6, s7, s8, s9}	;loading first five inputs all at one time
	;sumStoreCompare
	;bx lr								;return to calling function, only happens if the end condition is met
;loop
	;VMOV.F32 s5, s6						;moving each input down 
	;VMOV.F32 s6, s7
	;VMOV.F32 s7, s8
	;VMOV.F32 s8, s9
	;VLDR.F32 s9, [r0],#4				;loads the new input and increments the input pointer
	;sumStoreCompare
	;bx lr								;return to calling function, only happens if the end condition is met
;END

;The version below is ~20% faster than above but uses more space for the code
