


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
	 
static const float coeffsArrayAccel[25] = {-0.0006834905,-0.0018968938,-0.0013455906,0.0038953023,0.0113929899,0.0101893334,-0.0097993552,-0.0392034674,-0.0428766401,0.0161678021,0.1362264515,0.2610168359,0.3144090952,0.2610168359,0.1362264515,0.0161678021,-0.0428766401,-0.0392034674,-0.0097993552,0.0101893334,0.0113929899,0.0038953023,-0.0013455906,-0.0018968938,-0.0006834905};


void initAccelerometer(SPI_HandleTypeDef * handleSPI);	
void ledBrightness(float rollGoal, float pitchGoal);	 
float calibrateX(float rawX, float rawY, float rawZ);
float calibrateY(float rawX, float rawY, float rawZ);
float calibrateZ(float rawX, float rawY, float rawZ);
float pitch(float calX, float calY, float calZ);
float roll(float calX, float calY, float calZ);
void setBrightnessPitch(int level, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM);
void setBrightnessRoll(int level, TIM_HandleTypeDef * handleTIM, TIM_OC_InitTypeDef * initTIM);
float filterRoll(float newRollMeasurment);
float filterPitch(float newPitchMeasurment);
	 

	