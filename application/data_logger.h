#ifndef DATA_LOGGER_H
#define DATA_LOGGER_H

#include "FreeRTOS.h"
#include "task.h"

// Task Prototype
void vLoggingTask(void *pvParameters);

#endif