#include "ti_msp_dl_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h> 

#include "application/aqi_manager.h"
#include "application/data_logger.h"

QueueHandle_t xAQIQueue = NULL;

// Debug flag to simulate a hazardous AQI spike (Set via debugger)
uint8_t simulate_aqi_spike = 0;

/*
 * Maps a 12-bit raw ADC reading (0-4095) to standard AQI scale (0-500).
 * Uses fixed-point arithmetic to avoid FPU overhead.
 */
uint32_t Map_RawADC_to_AQI(uint32_t raw_adc_value) {
    if (simulate_aqi_spike) {
        return 450; // Force critical AQI level
    }

    if (raw_adc_value > 4095) {
        raw_adc_value = 4095; // Clamp to 12-bit max
    }

    return (raw_adc_value * 500) / 4095;
}

void vUARTTask(void* pvParameters) {
    char msg[32];

    // Force UART to enable right before the loop
    DL_UART_Main_enable(UART_0_INST);

    while (1) {
        // Convert raw moving average to standard AQI scale
        uint32_t final_aqi = Map_RawADC_to_AQI(aqiAverage);

        // Format and transmit AQI string
        int len = snprintf(msg, sizeof(msg), "AQI: %u\n", (unsigned int)final_aqi);

        for (int i = 0; i < len; i++) {
            while (DL_UART_isBusy(UART_0_INST));
            DL_UART_Main_transmitData(UART_0_INST, msg[i]);
        }

        // Block for 500ms to let ADC tasks run
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    SYSCFG_DL_init();

    xAQIQueue = xQueueCreate(10, sizeof(uint32_t));

    if (xAQIQueue != NULL) {
        // TASK HIERARCHY UPDATE:
        // Priority 4: UART (Must run to see data)
        // Priority 3: ADC Sampling (100Hz)
        // Priority 2: AES/CRC Security

        xTaskCreate(vUARTTask, "UART_Comm", 1024, NULL, 4, NULL);
        xTaskCreate(vProcessingTask, "ADC_Gen", 512, NULL, 3, NULL);
        xTaskCreate(vLoggingTask, "Logger", 512, NULL, 2, NULL);

        vTaskStartScheduler();
    }
    while (1) { __BKPT(0); }
}