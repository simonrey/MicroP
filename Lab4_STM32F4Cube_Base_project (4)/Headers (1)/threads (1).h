#include "cmsis_os.h"     


osThreadId startTempThread(osThreadId tempThreadID);
osThreadId startAccelThread(osThreadId accelThreadID);
osThreadId startDisplayThread(osThreadId displayThreadID);
osThreadId startKeypadThread(osThreadId keypadThreadID);
void setAccelerometerValuesReady(void);
