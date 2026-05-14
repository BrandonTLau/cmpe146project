#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "ti_msp_dl_config.h"
#include "aqi_manager.h"

uint32_t aqiAverage = 0; 

void vProcessingTask(void *pvParameters) {
    uint32_t rawSample = 0;
    while(1) {
        // Simulation of ADC data moving between 100 and 900
        static int val = 300;
        val += 5;
        if(val > 900) val = 100;
        
        aqiAverage = (uint32_t)val;
        
        // Pass to the secure logger
        xQueueSend(xAQIQueue, &aqiAverage, 0);
        
        // 100Hz Sampling Rate
        vTaskDelay(pdMS_TO_TICKS(10)); 
    }
}