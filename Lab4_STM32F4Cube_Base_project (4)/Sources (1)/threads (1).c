/*******************************************************************************
  * @file    Thread_LED.c
  * @author  Ashraf Suyyagh
	* @version V1.0.0
  * @date    17-January-2016
  * @brief   This file initializes one LED as an output, implements the LED thread 
  *					 which toggles and LED, and function which creates and starts the thread	
  ******************************************************************************
  */
	
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "stm32f4xx_hal.h"
#include "main.h"
#include "gpio.h"
#include "adc.h"
#include "accelerometer.h"

int accelerometerValuesReady  = 0;
int currentDigit 							= 0;
int	currentDisplayTemp				= 0;
int counterDisplayTempUpdate	= 0;
int isBlinking								= 0;

void keypadThread(void const *argument);
osThreadDef(keypadThread, osPriorityNormal, 1, 0);

void displayThread(void const *argument);
osThreadDef(displayThread, osPriorityHigh, 1, 0);

void tempThread(void const *argument);
osThreadDef(tempThread, osPriorityNormal, 1, 0);

void accelThread(void const *argument);
osThreadDef(accelThread, osPriorityNormal, 1, 0);

void Thread_LED (void const *argument);                 // thread function
osThreadId tid_Thread_LED;                              // thread id
osThreadDef(Thread_LED, osPriorityNormal, 1, 0);
GPIO_InitTypeDef 				LED_configuration;

/*----------------------------------------------------------------------------
 *      Create the thread within RTOS context
 *---------------------------------------------------------------------------*/
int start_Thread_LED (void) {

  tid_Thread_LED = osThreadCreate(osThread(Thread_LED ), NULL); // Start LED_Thread
  if (!tid_Thread_LED) return(-1); 
  return(0);
}

 /*----------------------------------------------------------------------------
*      Thread  'LED_Thread': Toggles LED
 *---------------------------------------------------------------------------*/
	void Thread_LED (void const *argument) {
		while(1){
				osDelay(1000);
				HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
			}
	}
/*----------------------------------------------------------------------------
 *      Initialize the GPIO associated with the LED
 *---------------------------------------------------------------------------*/
	void initializeLED_IO (void){
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	LED_configuration.Pin		= GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
	LED_configuration.Mode 	= GPIO_MODE_OUTPUT_PP;
	LED_configuration.Speed	= GPIO_SPEED_FREQ_VERY_HIGH;
	LED_configuration.Pull	= GPIO_NOPULL;
	HAL_GPIO_Init(GPIOD, &LED_configuration);	
}
	
/*----------------------------------------------------------------------------
 *    Temperature Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startTempThread(osThreadId tempThreadID){
	tempThreadID = osThreadCreate(osThread(tempThread ), NULL);
	if(!tempThreadID) return NULL;
	else return(tempThreadID);
}
/*----------------------------------------------------------------------------
 *    Temperature thread content 
 *---------------------------------------------------------------------------*/
void tempThread(void const *argument){
	initADC(getHandleADC(),getHandleADCChannel(),getHandleDMA(),getReadingADCLocation());
	while(1){
		osDelay(10);
		filter();
	}
}
/*----------------------------------------------------------------------------
 *    Accelerometer Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startAccelThread(osThreadId accelThreadID){
	accelThreadID = osThreadCreate(osThread(accelThread ), NULL);
	if(!accelThreadID) return NULL;
	else return(accelThreadID);
}
/*----------------------------------------------------------------------------
 *    Accelerometer thread content 
 *---------------------------------------------------------------------------*/
void accelThread(void const *argument){
	initAccelerometer(getHandleSPI());
	while(1){
		if(accelerometerValuesReady)
		{
			ledBrightness(getCurrentTargetRollAngle(),getCurrentTargetPitchAngle());
			accelerometerValuesReady = 0;
		}
	}
}

void setAccelerometerValuesReady(void)
{
	accelerometerValuesReady = 1;
}
/*----------------------------------------------------------------------------
 *    Display Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startDisplayThread(osThreadId displayThreadID){
	displayThreadID = osThreadCreate(osThread(displayThread), NULL);
	if(!displayThreadID) return NULL;
	else return(displayThreadID);
}
/*----------------------------------------------------------------------------
 *    Display thread content 
 *---------------------------------------------------------------------------*/
void displayThread(void const *argument){
	while(1){
		osDelay(1);
			currentDigit++;
			if(currentDigit > 3)
			{
				currentDigit = 1;
			}
			if(getIsDisplayingTargetAngle())
			{
				if(getTempTargetPitchAngle() == 0)
				{
					updateDigit(getTempTargetRollAngle()*10,currentDigit);
				}
				else
				{
					updateDigit(getTempTargetPitchAngle()*10,currentDigit);
				}
			}
			else
			{
				updateDigit(currentDisplayTemp,currentDigit);
			}
			
			if(currentDisplayTemp > 3500)
			{
				if(isBlinking > 500)
				{
					resetDigitSelectLines();
					counterDisplayTempUpdate = 0;
					isBlinking--;
				}
				else
				{
					if(isBlinking == 498)
					{
						counterDisplayTempUpdate = 201;
					}						
					
					isBlinking--;
					if(isBlinking < 1)
					{
						isBlinking = 1000;
					}
				} 
			}
			
			//every nth tick poll for new temperature
			counterDisplayTempUpdate++;
			if(counterDisplayTempUpdate >= 200)
			{
				counterDisplayTempUpdate = 0;
				// if the blue button is pressed give results in fahrenheit
				if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0))
				{
					currentDisplayTemp = (int)(((getCurrentTemp())*1.8 + 32.0)*100.0);
				}
				else
				{
					currentDisplayTemp = (int)((getCurrentTemp())*100.0);;
				}
			}
	}
}
/*----------------------------------------------------------------------------
 *    Keypad Thread Context creation  
 *---------------------------------------------------------------------------*/
osThreadId startKeypadThread(osThreadId keypadThreadID){
	keypadThreadID = osThreadCreate(osThread(keypadThread), NULL);
	if(!keypadThreadID) return NULL;
	else return(keypadThreadID);
}
/*----------------------------------------------------------------------------
 *    Keypad thread content 
 *---------------------------------------------------------------------------*/
void keypadThread(void const * argument){
	//thread content
	int row 						= 0;
	int column 					= 0;
	int number 					= 0;
	int currentDigit    = 0;
	int	keyWasPressed 	= 0;
	int starCount				= 0;

	while (1)
    {
        row = getRowSelected();

				
				
        if(row != 0 && keyWasPressed == 0)
        {
            initKeypadGPIO_2(getKeypadGPIO());
            column = getColumnSelected();
            initKeypadGPIO_1(getKeypadGPIO());
						setIsDisplayingTargetAngle(1); // display temp target angle
        }
        
        if(currentDigit == 0)
        {
            setTempTargetRollAngle(0);
            setTempTargetPitchAngle(0);
        }
            
        if(keyWasPressed > 0 && row == 0)
        {
            keyWasPressed--;
        }
        
        // handles numbers #
        if(row == 4 && column == 3 && keyWasPressed == 0)
        {
            if(currentDigit <4 && currentDigit != 0)
            {
                currentDigit = 4;
            }
            else
            {
								setCurrentTargetRollAngle(getTempTargetRollAngle());
								setCurrentTargetPitchAngle(getTempTargetPitchAngle());
                setIsDisplayingTargetAngle(0); // back to displaying emperatures
                //printf("%i,%i \n",getCurrentTargetRollAngle(),getCurrentTargetPitchAngle());
                row = 0;
                column = 0;
                currentDigit = 0;
                setTempTargetRollAngle(0);
                setTempTargetPitchAngle(0);
            }
            
            keyWasPressed = 1000;
        }
        // handles numbers *
        if(row == 4 && column == 1 && keyWasPressed == 0)
        {
            if(currentDigit <4 && currentDigit > 0)
            {
                setTempTargetRollAngle(getTempTargetRollAngle()/10);
                currentDigit--;
            }
            else if (currentDigit >4)
            {

								currentDigit--;
                setTempTargetPitchAngle(getTempTargetPitchAngle()/10);
            }
            
            keyWasPressed = 1000;
            //printf("%i,%i \n",getCurrentTargetRollAngle(),getCurrentTargetPitchAngle());
        }
        else if(row == 4 && column == 1 && keyWasPressed != 0)
        {
            starCount++;
            if(starCount > 600000)
            {
                starCount = 0;
                row = 0;
                column = 0;
                currentDigit = 0;
                setTempTargetRollAngle(0);
                setTempTargetPitchAngle(0);
								setCurrentTargetRollAngle(0);
                setCurrentTargetPitchAngle(0);
								setIsDisplayingTargetAngle(0);
                //printf("star count complete \n");
            }
                
        }
        
        // handles numbers 0
        if(row == 4 && column == 2 && keyWasPressed == 0 && currentDigit < 7 && currentDigit != 3)
        {
            keyWasPressed = 1000;
            number = 0;
            if(currentDigit <3 && currentDigit > 0)
            {
                setTempTargetRollAngle(getTempTargetRollAngle()*10);
                currentDigit++;
            }
            else
            {
							if(currentDigit > 4)
							{
								setTempTargetPitchAngle(getTempTargetPitchAngle()*10);
                currentDigit++;
							}
            }

            //printf("%i,%i \n",getCurrentTargetRollAngle(),getCurrentTargetPitchAngle());
            row = 0;
            column = 0;
        }
        
        // handles numbers 1-9
        if(row != 0 && column != 0 && keyWasPressed == 0 && column != 4 && row != 4 && currentDigit < 7 && currentDigit != 3)
        {
            keyWasPressed = 1000;
            number = ((row - 1) * 3) + column;
            if(currentDigit <3)
            {
                setTempTargetRollAngle(getTempTargetRollAngle()*10 + number);
                currentDigit++;
            }
            else
            {
                setTempTargetPitchAngle(getTempTargetPitchAngle()*10 + number);
                currentDigit++;
            }

            //printf("%i,%i \n",getCurrentTargetRollAngle(),getCurrentTargetPitchAngle());
            row = 0;
            column = 0;
        }
	}
}


