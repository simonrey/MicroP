
int FIR_C(float* InputArray, float* OutputArray, FIR_coeff* coeff,int blockSize, int order)
{
	// loop through the blockSize of the input array
	for(int i = 0; i < (blockSize - order); i++)
	{
		OutputArray[i] = coeff->b0*InputArray[i] + coeff->b1*InputArray[i+1] + coeff->b2*InputArray[i+2] + coeff->b3*InputArray[i+3] + coeff->b4*InputArray[i+4];
	}
	
	return 0;
}