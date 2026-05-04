#ifndef APP_SIGNALS_H
#define APP_SIGNALS_H

#include <FreeRTOS.h>
#include <semphr.h>

// This semaphore signals that 10 samples are ready in SRAM
extern SemaphoreHandle_t xSamplingSemaphore;

// This is the buffer where the DMA is dumping the ADC results
extern uint16_t gADCBuffer[10]; 

#endif