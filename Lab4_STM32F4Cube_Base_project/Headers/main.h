



uint32_t getReadingADC(void);
ADC_HandleTypeDef * getHandleADC(void);
DMA_HandleTypeDef * getHandleDMA(void);
float getCurrentTemp(void);
void setCurrentTemp(float t);
ADC_ChannelConfTypeDef * getHandleADCChannel(void);
uint32_t * getReadingADCLocation(void);
SPI_HandleTypeDef * getHandleSPI(void);
float getRollGoal(void);
float getPitchGoal(void);
TIM_HandleTypeDef * getHandleTIM(void);
TIM_OC_InitTypeDef * getInitTIM(void);
int getMode(void);