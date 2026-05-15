#ifndef APPLICATION_AQI_MANAGER_H_
#define APPLICATION_AQI_MANAGER_H_
 
#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
 
/* -----------------------------------------------------------------------
 * Task Prototypes
 * ----------------------------------------------------------------------- */
 
/* Processing Task (Priority 3 - HIGHEST)
 * Waits on xSamplingSemaphore, feeds WWDT, runs the MAF, forwards data. */
void vProcessingTask(void *pvParameters);
 
/* Simulator Task (Priority 4 - runs only in simulation builds)
 * Fills gADCBuffer with a ramp waveform and gives xSamplingSemaphore every
 * 10 ms, mimicking the DMA_IRQHandler that fires on real hardware. */
void vSimulatorTask(void *pvParameters);
 
/* -----------------------------------------------------------------------
 * Shared globals (defined in aqi_manager.c)
 * ----------------------------------------------------------------------- */
 
/* AQI queue: Processing Task → Logging Task (every 60 s / 6000 samples) */
extern QueueHandle_t xAQIQueue;
 
/* Moving-average result, also read directly by the UART task */
extern uint32_t aqiAverage;
 
#endif /* APPLICATION_AQI_MANAGER_H_ */
 