#include <stdio.h>
#include "arm_math.h"


typedef struct 
{
	float32_t b0,b1,b2,b3,b4;
}FIR_coeff;

//unsigned FIRasm(float* InputArray, float* OutputArray, FIR_coeff* coeff,int blockSize, int order);
float32_t FIR_asm(float32_t * InputArray, float32_t* OutputArray,int blockSize, float32_t* coeffs);


int FIR_C(float* InputArray, float* OutputArray, FIR_coeff* coeff ,int blockSize, int order)
{
	// loop through the blockSize of the input array
	for(int i = 0; i < (blockSize - order); i++)
	{
		OutputArray[i] = coeff->b0*InputArray[i] + coeff->b1*InputArray[i+1] + coeff->b2*InputArray[i+2] + coeff->b3*InputArray[i+3] + coeff->b4*InputArray[i+4];
	}
	
	return 0;
}

int main()
{
	int blockSize = 7; //this should be dynamic based on the size of the array being inputed
	int order 		= 4; //number of coeffs - 1
	int numTaps 	= order + 1; // this should be based on the number of coefficients in the FIR_coeff struct
	
	float32_t inputs[] = {0.5,0.9,0.34,0.69,0.34,0.12,0.89};
	// coeffs as struct and array
	FIR_coeff coeffs;
	coeffs.b0 = 0.1;
	coeffs.b1 = 0.15;
	coeffs.b2 = 0.5;
	coeffs.b3 = 0.15;
	coeffs.b4 = 0.1;
	float32_t coeffsArray[] = {0.1, 0.15, 0.5, 0.15, 0.1}; // should be numTaps +1 components in this array
	
//	// C SETUP

//	float32_t outputsC[blockSize - order];
//	
//	for(int i = 0; i < blockSize; i++)
//	{
//		printf("inputs %f \n",inputs[i]);
//	}
//	
//	FIR_C(inputs, outputsC, &coeffs, blockSize, order);
//	
//	for(int i = 0; i < (blockSize - order); i++)
//	{
//		printf("outputs from C %f \n",outputsC[i]);
//	}
//	
//	//CMSIS SETUP
	float32_t firState[numTaps + blockSize - 1]; // buffer used by arm_fir_init_f32()
	float32_t outputsCMSIS[blockSize];
	
	arm_fir_instance_f32 CMSISFir;
	
	arm_fir_init_f32(&CMSISFir, numTaps, &coeffsArray[0], &firState[0], blockSize);
	
	arm_fir_f32(&CMSISFir, inputs, outputsCMSIS, blockSize);
	
	for(int i = order; i < (blockSize); i++) // start at i = order since CMSIS FIR starts by just looking at the first term of the inputs array
	{
		printf("outputs from CMSIS %f \n",outputsCMSIS[i]);
	}
	
	//asm setup
//	float32_t out = FIRasm(12.2);
//	
//	printf("outputs from CMSIS %f \n",out);
//	; r0, input, r1, output, r2, size, r3, inputcoeffs
//FIR_asm
	
	float32_t outputsASM[blockSize];
	FIR_asm( inputs, outputsASM, blockSize, coeffsArray);
	
	for(int i = 0; i < (blockSize-4); i++) // start at i = order since CMSIS FIR starts by just looking at the first term of the inputs array
	{
		printf("outputs from asm %f \n",outputsASM[i]);
	}
	
	return 1;
}

