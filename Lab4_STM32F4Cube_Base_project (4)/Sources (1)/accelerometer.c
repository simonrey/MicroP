//All the GPIO stuff goes here

#include "gpio.h"
#include <stdlib.h>
#include <math.h>
#include "main.h"
#include "LIS3DSH.h"
#include "accelerometer.h"


int pastRollIndex = 0;
int pastPitchIndex = 0;
float pastRoll[25];
float pastPitch[25];

void initAccelerometer(SPI_HandleTypeDef * handleSPI){
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 7,7);
	
	
	HAL_SPI_MspInit(handleSPI); // sets up gpio stuff for the SPI config and accelerometer interrupt pin
	
	// configure accelerameter settings
	LIS3DSH_InitTypeDef LIS3DSH_InitStruct;
	LIS3DSH_InitStruct.AA_Filter_BW								= LIS3DSH_AA_BW_50;
	LIS3DSH_InitStruct.Axes_Enable								= LIS3DSH_XYZ_ENABLE;
	LIS3DSH_InitStruct.Continous_Update						= LIS3DSH_ContinousUpdate_Enabled;
	LIS3DSH_InitStruct.Full_Scale									= LIS3DSH_FULLSCALE_2;
	LIS3DSH_InitStruct.Power_Mode_Output_DataRate	= LIS3DSH_DATARATE_25;
	LIS3DSH_InitStruct.Self_Test									= LIS3DSH_SELFTEST_NORMAL;
	LIS3DSH_Init(&LIS3DSH_InitStruct); // must be called before LIS3DSH_DataReadyInterruptConfig, since this also sets up the SPI configuration
	
	// configure accelerometer interrupt
	LIS3DSH_DRYInterruptConfigTypeDef LIS3DSH_IntConfigStruct;
	LIS3DSH_IntConfigStruct.Dataready_Interrupt	= LIS3DSH_DATA_READY_INTERRUPT_ENABLED;
	LIS3DSH_IntConfigStruct.Interrupt_signal		= LIS3DSH_ACTIVE_HIGH_INTERRUPT_SIGNAL;
	LIS3DSH_IntConfigStruct.Interrupt_type			= LIS3DSH_INTERRUPT_REQUEST_PULSED;
	LIS3DSH_DataReadyInterruptConfig(&LIS3DSH_IntConfigStruct);
}

void ledBrightness(float rollGoal, float pitchGoal)
{
	float acceleration[3];
	LIS3DSH_ReadACC(acceleration);
	
	// takes the raw acceleration data and multiplies it by the calibration matrix
	float calX = calibrateX(acceleration[0],acceleration[1],acceleration[2]);
	float calY = calibrateY(acceleration[0],acceleration[1],acceleration[2]);
	float calZ = calibrateZ(acceleration[0],acceleration[1],acceleration[2]);
	
	// filters the roll and pitch values, finds their difference from their goals
	float rollResult 	= fabs(filterRoll(roll(calX,calY,calZ))) - rollGoal;
	float pitchResult   = fabs(filterPitch(pitch(calX,calY,calZ))) - pitchGoal;
	
	//determine pulse level for pwm based on angle difference
	float levelPitch = (32.0 * pitchResult/180.0);
	
	// get absolute value
	if(levelPitch<0)
	{
		levelPitch = -1*levelPitch;
	}
	
	// saturate at 30
	if(levelPitch>30)
	{
		levelPitch = 30;
	}
	
	float levelRoll = (32.0 * rollResult/180.0);
	
	if(levelRoll<0)
	{
		levelRoll = -1*levelRoll;
	}
	
	if(levelRoll>30)
	{
		levelRoll = 30;
	}
	
	// set the leds pwm pulse levels
	setBrightnessRoll(levelRoll, getHandleTIM(), getInitTIM());
	setBrightnessPitch(levelPitch, getHandleTIM(), getInitTIM());
	}

float calibrateX(float rawX, float rawY, float rawZ){
	return (rawX*0.001011535773528 + rawY*-0.000021375722414 + rawZ*-0.000002617250425 -0.024434131248271); 
}

float calibrateY(float rawX, float rawY, float rawZ){
	return (rawX*-0.000022173875989 + rawY*0.000959285589914 + rawZ*-0.000004686872148 -0.010543316906594); 
}

float calibrateZ(float rawX, float rawY, float rawZ){
	return (rawX*-0.000003976693405 + rawY*-0.000008559170730 + rawZ*0.000991525335892 -0.020677895349495); 
}

float pitch(float calX, float calY, float calZ){
	float y2 = pow(calY,2.0);
	float z2 = pow(calZ,2.0);
	float sqrtY2Z2 = sqrt(y2 + z2);
	
	return (atan(calX/sqrtY2Z2)*180.0/3.14159);
}

float roll(float calX, float calY, float calZ){
	float x2 = pow(calX,2.0);
	float z2 = pow(calZ,2.0);
	float sqrtX2Z2 = sqrt(x2 + z2);
	
	return (atan(calY/sqrtX2Z2)*180.0/3.14159);
}

void setBrightnessPitch(int level, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM){
	initTIM->Pulse = (level);
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(handleTIM,initTIM,TIM_CHANNEL_3);

	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_3);
}

void setBrightnessRoll(int level, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM){
	initTIM->Pulse = (level);
	HAL_TIM_PWM_ConfigChannel(handleTIM, initTIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(handleTIM, initTIM,TIM_CHANNEL_4);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(handleTIM,TIM_CHANNEL_4);
}


float filterRoll(float newRollMeasurment){
	pastRoll[pastRollIndex] = newRollMeasurment;
	pastRollIndex++;
	if(pastRollIndex > 24)
	{
		pastRollIndex = 0;
	}
			
	float scratchRollMeasurment = 0;
	for(int i = pastRollIndex; i < 25; i++)
	{
		scratchRollMeasurment = scratchRollMeasurment + pastRoll[i]*coeffsArrayAccel[i - pastRollIndex];
	}
			
	for(int i = 0; i < pastRollIndex; i++)
	{
		scratchRollMeasurment = scratchRollMeasurment + pastRoll[i]*coeffsArrayAccel[i + (25 - pastRollIndex)];
	}
		
	return scratchRollMeasurment;
}

float filterPitch(float newPitchMeasurment){
	pastPitch[pastPitchIndex] = newPitchMeasurment;
	pastPitchIndex++;
	if(pastPitchIndex > 24)
	{
		pastPitchIndex = 0;
	}
		
	float scratchPitchMeasurment = 0;
	for(int i = pastPitchIndex; i < 25; i++)
	{
		scratchPitchMeasurment = scratchPitchMeasurment + pastPitch[i]*coeffsArrayAccel[i - pastPitchIndex];
	}
			
	for(int i = 0; i < pastPitchIndex; i++)
	{
		scratchPitchMeasurment = scratchPitchMeasurment + pastPitch[i]*coeffsArrayAccel[i + (25 - pastPitchIndex)];
	}
	
	return scratchPitchMeasurment;
}



