#ifndef APPLICATION_AQI_MANAGER_H_
#define APPLICATION_AQI_MANAGER_H_

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"

// Prototypes
void vProcessingTask(void *pvParameters);

// Global queue handle defined in main.c
extern QueueHandle_t xAQIQueue;

// Global variable for UART broadcast
extern uint32_t aqiAverage;

#endif