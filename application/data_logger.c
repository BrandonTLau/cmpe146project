#include "data_logger.h"
#include "ti_msp_dl_config.h"
#include "queue.h"
#include <string.h>

extern QueueHandle_t xAQIQueue; 

void vLoggingTask(void *pvParameters) {
    uint32_t receivedAQI;
    uint32_t encryptedAQI;
    
    /* * Senior Project Note: Hardware AES integration in progress.
     * Currently using a software XOR cipher for data obfuscation.
     */
    const uint32_t SECRET_KEY = 0xDEADBEEF; 
    
    while(1) {
        /* Wait for data from the Processing Task */
        if (xQueueReceive(xAQIQueue, &receivedAQI, pdMS_TO_TICKS(1000)) == pdPASS) {
            
            /* 1. Obfuscate/Encrypt the data */
            encryptedAQI = receivedAQI ^ SECRET_KEY;

            /* 2. Success Indicator */
            // Rapid blink indicates a secure log entry was "processed"
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
            
            /* In a final build, 'encryptedAQI' would be saved to Flash memory. */
            
        } else {
            /* Slow heartbeat (1 second) if no data is flowing */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }
    }
}