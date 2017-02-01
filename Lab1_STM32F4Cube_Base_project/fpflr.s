	AREA FIR_Filter, CODE, READONLY
	
	EXPORT FIR_asm
	 ;;r0, input, r1, output, r2, size, r3, inputcoeffs
	
	 ;;s5, used to accumulate output values
	 ;;r6, used to store the end condition

	
FIR_asm						;FIR_asm(float32_t * InputArray, float32_t* OutputArray,int blockSize, float32_t* coeffs)

	MOV r6, #4				;set r6 to 4, this is required for the MUL as it cant take constants
	SUB r2, r2, #4			;set r2 minus the order, 4, gives the number of loops needed to cover entire input array
	MUL r2, r2, r6			;multiply the number of loops needed by 4, gives the number of bytes the ouput pointer will be incremented by the end
	ADD r6, r1, r2			;add the numberof bytes to be incremented to the address of output[0] 
	
	
start
	VLDR.F32 s6, [r3,#0] 	;puts the first coeff in s6, needs to be in a register for VMUL to take it 
	VLDR.F32 s7, [r0,#0] 	;puts the first input value corresponding to the current output in s7
	VMUL.F32 s5, s6, s7		;multiplies s6 by s7 and puts the result in s5, accumulate not used here since this is the first term in the sum for this output 
	
	VLDR.F32 s6, [r3,#4] 	;sets s6 to the second coeff
	VLDR.F32 s7, [r0,#4] 	;sets s7 to the second input for the current output
	VFMA.F32 s5, s6, s7		;multiplies s6 by s7 and adds the result to whats currently in s5
	
	VLDR.F32 s6, [r3,#8] 
	VLDR.F32 s7, [r0,#8] 
	VFMA.F32 s5, s6, s7

	VLDR.F32 s6, [r3,#12] 	;IF SYMMETRICAL NOT NEEDED
	VLDR.F32 s7, [r0,#12] 	;IF SYMMETRICAL NOT NEEDED 
	VFMA.F32 s5, s6, s7
	
	VLDR.F32 s6, [r3,#16] 	;IF SYMMETRICAL NOT NEEDED 
	VLDR.F32 s7, [r0,#16] 	;IF SYMMETRICAL NOT NEEDED 
	VFMA.F32 s5, s6, s7
	
	VSTR.F32 s5, [r1]		;adds the current output sum to its array
	
	ADD r1,r1,#4			;moves the pointer to the output array to the next entry location
	ADD r0,r0,#4			;moves the pointer to the first input value to correspond with the next output value
	CMP r1, r6				;compares the pointer value of the output array to its end condition
	beq return				;if it meets the end condition return to the calling function
	
	b start 				;otherwise return to calculating the next output value
	
return
	bx lr
	END