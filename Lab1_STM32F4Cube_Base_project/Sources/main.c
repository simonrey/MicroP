#include <stdio.h>
#include "arm_math.h"

typedef struct 
{
	float32_t b0,b1,b2,b3,b4;
}FIR_coeff;

float32_t FIR_asm(float32_t * InputArray, float32_t* OutputArray,int blockSize, float32_t* coeffs);

void FIR_C(float32_t* InputArray, float32_t* OutputArray, FIR_coeff* coeff ,int blockSize, int order)
{
	for(int i = 0; i < (blockSize - order); i++)
	{
		OutputArray[i] = coeff->b0*InputArray[i] + coeff->b1*InputArray[i+1] + coeff->b2*InputArray[i+2] + coeff->b3*InputArray[i+3] + coeff->b4*InputArray[i+4];
	}
}

int main()
{
	int blockSize = 7; //this should be dynamic based on the size of the array being inputed
	int order 		= 4; //number of coeffs - 1
	int numTaps 	= order + 1; // this should be based on the number of coefficients in the FIR_coeff struct
	
	//common inputs
	float32_t inputs[] = {0.5,0.9,0.34,0.69,0.34,0.12,0.89};
	
	// coeffs as struct and array
	FIR_coeff coeffs;
	coeffs.b0 = 0.1;
	coeffs.b1 = 0.15;
	coeffs.b2 = 0.5;
	coeffs.b3 = 0.15;
	coeffs.b4 = 0.1;
	float32_t coeffsArray[] = {0.1, 0.15, 0.5, 0.15, 0.1}; // should be numTaps +1 components in this array
	
	//seperate arrays for output so they can be compared after
	float32_t outputsCMSIS[blockSize];
	float32_t outputsASM[blockSize];
	float32_t outputsC[blockSize - order];
	

// C FIR
	printf("starting C\n");
	FIR_C(inputs, outputsC, &coeffs, blockSize, order);
	printf("ending C\n");

//ASM FIR		
	printf("starting ASM\n");
	FIR_asm(inputs, outputsASM, blockSize, coeffsArray);
	printf("ending ASM\n");

//CMSIS FIR
	printf("starting CMSIS\n");
	float32_t firState[numTaps + blockSize - 1]; // buffer used by arm_fir_init_f32()
	
	
	arm_fir_instance_f32 CMSISFir;
	arm_fir_init_f32(&CMSISFir, numTaps, &coeffsArray[0], &firState[0], blockSize);
	arm_fir_f32(&CMSISFir, inputs, outputsCMSIS, blockSize);
	
	printf("ending CMSIS %f \n", outputsCMSIS[order] );
// print outputs
	for(int i = 0; i < (blockSize - order); i++) 
	{
		printf("outputs from CMSIS %f, C %f, asm %f, i %i, blockSize %i, order %i \n",outputsCMSIS[i + order], outputsC[i], outputsASM[i], i, blockSize, order);
	}

// Part 2
	
// calculating differences
	float32_t differenceCMSIS[blockSize - order];
	float32_t differenceC[blockSize - order];
	float32_t differenceASM[blockSize - order];
		
	printf("calculating differences\n");
	for(int i = 0; i < (blockSize - order); i++) 
	{
		differenceCMSIS[i] 	= inputs[i] - outputsCMSIS[i + order];
		differenceC[i] 			= inputs[i] - outputsC[i];
		differenceASM[i] 		= inputs[i] - outputsASM[i];
	}
	printf("finished calculating differences\n");
	
// calculating averages
	float32_t averageCMSIS = 0;
	float32_t averageC = 0;
	float32_t averageASM = 0;
	
	for(int i = 0; i < (blockSize - order); i++) 
	{
		averageCMSIS = differenceCMSIS[i] + averageCMSIS;
		averageC		 = differenceC[i] 		+ averageC;
		averageASM	 = differenceASM[i] 	+ averageCMSIS;
	}
	
	averageCMSIS	= (averageCMSIS/(float)(blockSize - order));
	averageC 			= (averageC/(float)(blockSize - order));
	averageASM 		= (averageASM/(float)(blockSize - order));
	
	printf("cmsis %f, c %f, asm %f", averageCMSIS, averageC, averageASM);
// calculating standard deviations
	
// calculating correlations
	
	return 0;
}

