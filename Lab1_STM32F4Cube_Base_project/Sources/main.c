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
	static const int blockSize = 20; //this should be dynamic based on the size of the array being inputed
	static const int order 		= 4; //number of coeffs - 1
	static const int numTaps 	= order + 1; // this should be based on the number of coefficients in the FIR_coeff struct
	
	//common inputs
	float32_t inputs[20] = {1.0, 1.07, 1.15, 1.2, 1.25, 1.3, 1.358, 1.39, 1.15, 1.2, 1.15, 1.1, 1.05, 1.0, 0.8, 0.6, 0.4, 0.0, -0.3, -0.8};
	
	// coeffs as struct and array
	FIR_coeff coeffs;
	coeffs.b0 = 0.1;
	coeffs.b1 = 0.15;
	coeffs.b2 = 0.5;
	coeffs.b3 = 0.15;
	coeffs.b4 = 0.1;
	float32_t coeffsArray[numTaps] = {0.1, 0.15, 0.5, 0.15, 0.1}; // should be numTaps +1 components in this array
	
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

		
	for(int i = 0; i < (blockSize - order); i++) 
	{
		differenceCMSIS[i] 	= inputs[i] - outputsCMSIS[i + order];

	}
	printf("\nC Statistic Implementation Results\n");
	for(int i = 0; i < blockSize-order; i++){
		//printf("Differences - ASM: %f\tC: %f\tCMSIS: %f\n", differenceASM[i], differenceC[i], differenceCMSIS[i]);
		printf("Differences - CMSIS: %f\n", differenceCMSIS[i]);
	}
	
// calculating averages
	float32_t averageCMSIS = 0;
	float32_t averageInput = 0;
	float32_t averageOutputCMSIS = 0;
	//summing
	for(int i = 0; i < (blockSize - order); i++) 
	{
		averageInput = inputs[i] + averageInput;
		averageOutputCMSIS= outputsCMSIS[i + order] + averageOutputCMSIS;
		averageCMSIS = differenceCMSIS[i] + averageCMSIS;
	}
	//dividing by number of terms summed
	averageInput = (averageInput/(float)(blockSize-order));
	averageOutputCMSIS = (averageOutputCMSIS/(float)(blockSize-order));
	averageCMSIS = (averageCMSIS/(float)(blockSize - order));
	
	printf("Mean - CMSIS: %f\n",averageCMSIS);

	// calculating standard deviations
	float32_t stdCMSIS = 0;
	float32_t stdOutputCMSIS = 0;
	float32_t stdInput = 0;
	
	//calculating (difference - average)^2
	for(int i = 0; i < (blockSize - order); i++) 
	{
		stdCMSIS = pow((differenceCMSIS[i] 	- averageCMSIS), 2) + stdCMSIS;
		stdOutputCMSIS = pow((outputsCMSIS[i + order] - averageOutputCMSIS), 2) + stdOutputCMSIS;
		stdInput = pow((inputs[i]-averageInput), 2) + stdInput;
	}

	stdCMSIS	= (stdCMSIS/(float)(blockSize - order));
	stdCMSIS	=	sqrt(stdCMSIS);
	
	stdOutputCMSIS = (stdOutputCMSIS/(float)(blockSize-order));
	stdInput = (stdInput/(float)(blockSize-order));

	stdOutputCMSIS = sqrt(stdOutputCMSIS);
	stdInput = sqrt(stdInput);

	printf("Standard Deviations -\tCMSIS: %f\n",stdCMSIS);

// calculating correlation coeff
	float32_t correlationCMSIS = 0;

	for(int i = 0; i < blockSize-order; i++){
		correlationCMSIS = (inputs[i]-averageInput) * (outputsCMSIS[i+order]-averageOutputCMSIS) + correlationCMSIS;	
	}
	
	correlationCMSIS = correlationCMSIS / ((stdInput * stdOutputCMSIS)*(blockSize-order));
		
	printf("Correlation - ASM:CMSIS: %f\n", correlationCMSIS);


	//CMSIS Part II
	
	//Part A - Taking Differences

	arm_sub_f32(inputs, &outputsCMSIS[order], differenceCMSIS, blockSize-order);
	
	printf("\nCMSIS DSP Lib results\n");
	for(int i = 0; i < blockSize-order; i++){
		printf("Differences - CMSIS: %f\n", differenceCMSIS[i]);
	}
	
	//Part B - Standard Deviation
	stdCMSIS = 0;
	arm_std_f32(differenceCMSIS, (blockSize-order), &stdCMSIS);
	printf("Standard Deviations - CMSIS: %f\n",stdCMSIS);

	//Part B - Find the mean of the differences
	averageCMSIS = 0;
	arm_mean_f32(differenceCMSIS, (blockSize-order), &averageCMSIS);
	printf("Mean - ASM:CMSIS: %f\n",averageCMSIS);

	//Part C - Find the correlation between original and tracked vectors

	float32_t correlationArrayCMSIS[((blockSize - order)*2 -1)];
	arm_correlate_f32(inputs, (blockSize-order), &outputsCMSIS[order], blockSize-order, correlationArrayCMSIS);
	for(int i = 0; i < ((blockSize - order)*2 -1); i++){
			printf("Correlation - CMSIS: %f\n",correlationArrayCMSIS[i]);
	}
	return 0;
}
