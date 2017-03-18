



uint32_t getReadingADC(void);
ADC_HandleTypeDef * getHandleADC(void);
DMA_HandleTypeDef * getHandleDMA(void);
ADC_ChannelConfTypeDef * getHandleADCChannel(void);
uint32_t * getReadingADCLocation(void);
SPI_HandleTypeDef * getHandleSPI(void);
TIM_HandleTypeDef * getHandleTIM(void);
TIM_OC_InitTypeDef * getInitTIM(void);
GPIO_InitTypeDef * getKeypadGPIO(void);

int getMode(void);
float getCurrentTemp(void);
void setCurrentTemp(float t);
float getRollGoal(void);
float getPitchGoal(void);

void setIsDisplayingTargetAngle(int newValue);
int getIsDisplayingTargetAngle(void);
void setTempTargetRollAngle(int newValue);
int getTempTargetRollAngle(void);
void setTempTargetPitchAngle(int newValue);
int getTempTargetPitchAngle(void);
void setCurrentTargetRollAngle(int newValue);
int getCurrentTargetRollAngle(void);
void setCurrentTargetPitchAngle(int newValue);
int getCurrentTargetPitchAngle(void);


