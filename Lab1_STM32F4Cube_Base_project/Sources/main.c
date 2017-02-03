#include <stdio.h>
#include <math.h>
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
		
	for(int i = 0; i < (blockSize - order); i++) 
	{
		differenceCMSIS[i] 	= inputs[i] - outputsCMSIS[i + order];
		differenceC[i] 			= inputs[i] - outputsC[i];
		differenceASM[i] 		= inputs[i] - outputsASM[i];
	}
	printf("\nC Statistic Implementation Results\n");
	for(int i = 0; i < blockSize-order; i++){
		printf("Differences - ASM: %f\tC: %f\tCMSIS: %f\n", differenceASM[i], differenceC[i], differenceCMSIS[i]);
	}
	
// calculating averages
	float32_t averageCMSIS 	= 0;
	float32_t averageC 			= 0;
	float32_t averageASM 		= 0;
	float32_t averageInput  = 0;
	float32_t averageOutputCMSIS = 0;
	float32_t averageOutputC = 0;
	float32_t averageOutputASM = 0;
	
	for(int i = 0; i < (blockSize - order); i++) 
	{
		averageInput = inputs[i]     + averageInput;
		averageOutputCMSIS= outputsCMSIS[i+order] + averageOutputCMSIS;
		averageOutputC = outputsC[i] + averageOutputC;
		averageOutputASM = outputsASM[i] + averageOutputASM;
		averageCMSIS = differenceCMSIS[i] + averageCMSIS;
		averageC		 = differenceC[i] 		+ averageC;
		averageASM	 = differenceASM[i] 	+ averageASM;
	}
	
	averageInput = (averageInput/(float)(blockSize-order));
	averageOutputCMSIS = (averageOutputCMSIS/(float)(blockSize-order));
	averageOutputC = (averageOutputC/(float)(blockSize-order));
	averageOutputASM = (averageOutputASM/(float)(blockSize - order));
	averageCMSIS	= (averageCMSIS/(float)(blockSize - order));
	averageC 			= (averageC/(float)(blockSize - order));
	averageASM 		= (averageASM/(float)(blockSize - order));
	
	//printf("average for input %f CMSIS %f, c %f, asm %f \n", averageInput, averageOutputCMSIS, averageOutputC, averageOutputASM);
	
	printf("Mean - ASM: %f\tC: %f\tCMSIS: %f\n", averageASM, averageC, averageCMSIS);

	// calculating standard deviations
	float32_t stdCMSIS 	= 0;
	float32_t stdC 			= 0;
	float32_t stdASM 		= 0;
	float32_t stdOutputC = 0;
	float32_t stdOutputASM = 0;
	float32_t stdOutputCMSIS = 0;
	float32_t stdInput = 0;
	
	//calculating (difference - average)^2
	for(int i = 0; i < (blockSize - order); i++) 
	{
		
		stdCMSIS = pow((differenceCMSIS[i] 	- averageCMSIS),2) 	+ stdCMSIS;
		stdC		 = pow((differenceC[i] 			- averageC),2) 			+ stdC;
		stdASM	 = pow((differenceASM[i] 		- averageASM),2) 		+ stdASM;
		stdOutputC = pow((outputsC[i]-averageOutputC),2) + stdOutputC;
		stdOutputASM = pow((outputsASM[i]-averageOutputASM),2) + stdOutputASM;
		stdOutputCMSIS = pow((outputsCMSIS[i+order]-averageOutputCMSIS),2) + stdOutputCMSIS;
		stdInput = pow((inputs[i]-averageInput),2) + stdInput;
		
	}

	stdCMSIS	= (stdCMSIS/(float)(blockSize - order));
	stdC 			= (stdC/(float)(blockSize - order));
	stdASM 		= (stdASM/(float)(blockSize - order));	
	stdCMSIS	=	sqrt(stdCMSIS);
	stdC			=	sqrt(stdC);
	stdASM		=	sqrt(stdASM);

	stdOutputC = (stdOutputC/(float)(blockSize-order));
	stdOutputASM = (stdOutputASM/(float)(blockSize-order));
	stdOutputCMSIS = (stdOutputCMSIS/(float)(blockSize-order));
	stdInput = (stdInput/(float)(blockSize-order));
	stdOutputC = sqrt(stdOutputC);
	stdOutputASM = sqrt(stdOutputASM);
	stdOutputCMSIS = sqrt(stdOutputCMSIS);
	stdInput = sqrt(stdInput);

	printf("Standard Deviations - ASM: %f\tC: %f\tCMSIS: %f\n", stdASM, stdC, stdCMSIS);

	
	float32_t correlationCMSIS = 0;
	float32_t correlationC = 0;
	float32_t correlationASM = 0;
	
	for(int i = 0; i < blockSize-order; i++){
		
		correlationCMSIS = (inputs[i]-averageInput) * (outputsCMSIS[i+order]-averageOutputCMSIS) + correlationCMSIS;
		correlationC = (inputs[i] - averageInput) * (outputsC[i] - averageOutputC) + correlationC;
		correlationASM = (inputs[i] - averageInput) * (outputsASM[i] - averageOutputASM) + correlationASM;
		
	}
	
	
	correlationCMSIS = correlationCMSIS / ((stdInput * stdOutputCMSIS)*(blockSize-order));
	correlationC = correlationC / ((stdInput * stdOutputC)*(blockSize-order));
	correlationASM = correlationASM / ((stdInput * stdOutputASM)*(blockSize-order));
		
	printf("Correlation - ASM: %f\tC: %f\tCMSIS: %f\n", correlationASM, correlationC, correlationCMSIS);


	//CMSIS Part II
	
	
	
	//Part A - Taking Differences
	arm_sub_f32(inputs, outputsASM, differenceASM, blockSize-order);
	arm_sub_f32(inputs, outputsC, differenceC, blockSize-order);
	arm_sub_f32(inputs, &outputsCMSIS[order], differenceCMSIS, blockSize-order);
	
	//Part B - Standard Deviation
	
	
	arm_std_f32(differenceASM, blockSize-order, &stdASM);
	arm_std_f32(differenceC, blockSize-order, &stdC);
	arm_std_f32(differenceCMSIS, blockSize-order, &stdCMSIS);
	
	//Part B - Find the mean of the differences
	
	arm_mean_f32(differenceASM, blockSize-order, &averageASM);
	arm_mean_f32(differenceC, blockSize-order, &averageC);
	arm_mean_f32(differenceCMSIS, blockSize-order, &averageCMSIS);
	
	//Part C - Find the correlation between original and tracked vectors
	
	correlationASM = 0;
	correlationC = 0;
	correlationCMSIS = 0;
	
	arm_correlate_f32(inputs, blockSize-order, outputsASM, blockSize-order, &correlationASM);
	arm_correlate_f32(inputs, blockSize-order, outputsC, blockSize-order, &correlationC);
	arm_correlate_f32(inputs, blockSize-order, &outputsCMSIS[order], blockSize-order, &correlationCMSIS);
	
	
	printf("\nCMSIS DSP Lib results\n");
	for(int i = 0; i < blockSize-order; i++){
		printf("Differences - ASM: %f\tC: %f\tCMSIS: %f\n", differenceASM[i], differenceC[i], differenceCMSIS[i]);
	}
	printf("Mean - ASM: %f\tC: %f\tCMSIS: %f\n", averageASM, averageC, averageCMSIS);
	printf("Standard Deviations - ASM: %f\tC: %f\tCMSIS: %f\n", stdASM, stdC, stdCMSIS);
	printf("Correlation - ASM: %f\tC: %f\tCMSIS: %f\n", correlationASM, correlationC, correlationCMSIS);
	
	
	
	
	
// calculating correlations
	
	return 0;
}

 