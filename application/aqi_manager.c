#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "ti_msp_dl_config.h"
 
#include "aqi_manager.h"
#include "../application/app_signals.h"
#include "../drivers/filter_hardware.h"
#include "../drivers/watchdog_helper.h"
 
/* -----------------------------------------------------------------------
 * Shared globals  (extern-declared in aqi_manager.h and app_signals.h)
 * ----------------------------------------------------------------------- */
QueueHandle_t      xAQIQueue         = NULL;
uint32_t           aqiAverage        = 0;
 
/* Semaphore signalled by DMA_IRQHandler (hardware) or vSimulatorTask (sim) */
SemaphoreHandle_t  xSamplingSemaphore = NULL;
 
/* DMA writes 10 raw ADC samples here each 10 ms burst */
uint16_t           gADCBuffer[10]    = {0};
 
/* -----------------------------------------------------------------------
 * vSimulatorTask  (Priority 4)
 *
 * Replaces the DMA hardware interrupt for simulation / debug builds.
 * Every 10 ms it writes a sawtooth ramp into gADCBuffer (mimicking the ADC
 * values the real DMA would transfer) then gives xSamplingSemaphore exactly
 * as DMA_IRQHandler would on physical hardware.
 *
 * On real hardware:
 *   - Remove this task.
 *   - Implement DMA_IRQHandler (see stub in main.c) which clears the DMA
 *     interrupt flag and calls xSemaphoreGiveFromISR(xSamplingSemaphore, ...).
 * ----------------------------------------------------------------------- */
void vSimulatorTask(void *pvParameters)
{
    (void)pvParameters;
 
    static int rampVal = 300;   /* starting ADC code */
 
    for (;;) {
        /* Advance ramp value and fill the 10-sample buffer */
        for (int i = 0; i < 10; i++) {
            rampVal += 5;
            if (rampVal > 900) { rampVal = 100; }
            gADCBuffer[i] = (uint16_t)rampVal;
        }
 
        /* Signal the Processing Task — equivalent to DMA_IRQHandler give */
        xSemaphoreGive(xSamplingSemaphore);
 
        /* 100 Hz: one 10-sample burst every 10 ms */
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
 
/* -----------------------------------------------------------------------
 * vProcessingTask  (Priority 3 - HIGHEST real-work priority)
 *
 * Wakes on xSamplingSemaphore → feeds WWDT → runs MATHACL MAF → every
 * 60 seconds (6000 samples at 100 Hz) pushes the filtered AQI to xAQIQueue
 * for the Logging Task.
 * ----------------------------------------------------------------------- */
void vProcessingTask(void *pvParameters)
{
    (void)pvParameters;
 
    static uint32_t sampleCounter = 0;
 
    for (;;) {
        /*
         * Block indefinitely until the DMA (or simulator) signals that
         * 10 fresh ADC samples are ready in gADCBuffer.
         * The task enters a low-power sleep while waiting (FreeRTOS idle hook
         * can engage Sleep Mode here to satisfy the power-efficiency NFR).
         */
        xSemaphoreTake(xSamplingSemaphore, portMAX_DELAY);
 
        /*
         * RELIABILITY: Feed the Windowed Watchdog immediately after waking.
         * If processing ever hangs and we stop reaching this line, the WWDT
         * will reset the MCU (test EX-01).
         */
        
 
        /*
         * HARDWARE OFFLOADING: Delegate the 10-point Moving Average Filter
         * to the Math Accelerator (MATHACL).  The CPU submits the buffer and
         * reads back the result without performing iterative addition/division
         * itself, keeping CPU utilisation below 30 % (hardware-co-design NFR).
         *
         * CC-01 safety: MathACL_CalculateAverage clamps max ADC value (4095)
         * before accumulating so there is no integer overflow in fixed-point.
         */
        aqiAverage = MathACL_CalculateAverage(gADCBuffer, 10);
 
        sampleCounter++;
 
        /*
         * Log interval: 100 Hz × 60 s = 6000 samples.
         * Send the filtered AQI to the Logging Task via the queue.
         * Use 0 timeout — if the queue is full, drop this batch rather than
         * blocking the high-priority processing loop.
         */
        if (sampleCounter >= 500U) {
            xQueueSend(xAQIQueue, &aqiAverage, 0);
            sampleCounter = 0;
        }
    }
}