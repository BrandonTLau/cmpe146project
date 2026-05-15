

#include "ti_msp_dl_config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include <stdio.h>
 
#include "application/app_signals.h"
#include "application/aqi_manager.h"
#include "application/data_logger.h"
#include "application/app_signals.h"
 
/* -----------------------------------------------------------------------
 * Task Priority Definitions  (matches design document §FreeRTOS Task Structure)
 *
 *  Priority 3 (HIGH)   : Processing Task  — DMA semaphore, WWDT, MAF
 *  Priority 2 (MEDIUM) : Logging Task     — AES-128 encryption, circular buffer
 *  Priority 1 (LOW)    : UART Task        — CLI output, CRC check
 *
 *  Priority 4          : Simulator Task   — SIMULATION ONLY, remove on HW build
 *                        (runs above Processing so it can always give the semaphore)
 *
 * NOTE: The original stub had UART at priority 4 so it would always print
 * during debug.  That is corrected here to match the real-time constraint:
 * the UART task must NEVER pre-empt the Processing Task (determinism NFR).
 * ----------------------------------------------------------------------- */
#define PRIORITY_PROCESSING   3
#define PRIORITY_LOGGING      2
#define PRIORITY_UART         1
#define PRIORITY_SIMULATOR    4   /* simulation only */
 
/* -----------------------------------------------------------------------
 * Map a 12-bit raw ADC value (0-4095) to the standard AQI scale (0-500).
 * Uses fixed-point arithmetic — no FPU required (Cortex-M0+ constraint).
 *
 * CC-01: raw_adc_value is clamped to 4095 before the multiply to prevent
 * integer overflow when computing (value × 500).
 * ----------------------------------------------------------------------- */
 
/* Debug flag: set to 1 via debugger to simulate a hazardous AQI spike */
uint8_t simulate_aqi_spike = 0;
 
uint32_t Map_RawADC_to_AQI(uint32_t raw_adc_value)
{
    if (simulate_aqi_spike) {
        return 450U;            /* force critical level for testing */
    }
    if (raw_adc_value > 4095U) {
        raw_adc_value = 4095U;  /* clamp — CC-01 */
    }
    return (raw_adc_value * 500U) / 4095U;
}
 
/* -----------------------------------------------------------------------
 * vUARTTask  (Priority 1 - LOWEST)
 *
 * Blocks on xUARTQueue waiting for an encrypted log entry from the Logging
 * Task.  Computes and verifies the CRC-32, then transmits the formatted
 * packet over UART (test FT-03, EX-03).
 *
 * Output format (matches pseudocode in design document):
 *   System Uptime: <ms>
 *   Secure Data:   <32 hex chars>
 *   Checksum:      <8 hex chars>
 * ----------------------------------------------------------------------- */
void vUARTTask(void *pvParameters)
{
    (void)pvParameters;
 
    AQILogEntry_t entry;
    char          buf[64];
    int           len;
 
    DL_UART_Main_enable(UART_0_INST);
        const char *hello = "HELLO\r\n";
        for (const char *p = hello; *p; p++) { while (DL_UART_isBusy(UART_0_INST)); DL_UART_Main_transmitData(UART_0_INST, (uint8_t)*p); }
 
    for (;;) {
        /* Block until the Logging Task pushes an encrypted packet */
        if (xQueueReceive(xUARTQueue, &entry, portMAX_DELAY) == pdPASS) {
 
            /* EX-03: Verify CRC before transmitting.
             * In a real build a CRC mismatch here indicates SRAM corruption. */
            uint32_t recomputed = CRC32_Calculate(entry.encrypted, AES_BLOCK_SIZE);
            if (recomputed != entry.crc32) {
                /* CRC mismatch — reject packet and signal error */
                const char *err = "ERROR: CRC mismatch, packet rejected\r\n";
                for (const char *p = err; *p; p++) {
                    while (DL_UART_isBusy(UART_0_INST));
                    DL_UART_Main_transmitData(UART_0_INST, (uint8_t)*p);
                }
                continue;
            }
 
            /* Uptime line */
            len = snprintf(buf, sizeof(buf),
                           "System Uptime: %lu ms\r\n",
                           (unsigned long)entry.uptime_ms);
            for (int i = 0; i < len; i++) {
                while (DL_UART_isBusy(UART_0_INST));
                DL_UART_Main_transmitData(UART_0_INST, (uint8_t)buf[i]);
            }
 
            /* Encrypted payload — 32 hex characters */
            len = snprintf(buf, sizeof(buf), "Secure Data:   ");
            for (int i = 0; i < len; i++) {
                while (DL_UART_isBusy(UART_0_INST));
                DL_UART_Main_transmitData(UART_0_INST, (uint8_t)buf[i]);
            }
            for (int i = 0; i < AES_BLOCK_SIZE; i++) {
                len = snprintf(buf, sizeof(buf), "%02X", entry.encrypted[i]);
                for (int j = 0; j < len; j++) {
                    while (DL_UART_isBusy(UART_0_INST));
                    DL_UART_Main_transmitData(UART_0_INST, (uint8_t)buf[j]);
                }
            }
            /* newline after hex */
            while (DL_UART_isBusy(UART_0_INST));
            DL_UART_Main_transmitData(UART_0_INST, '\r');
            while (DL_UART_isBusy(UART_0_INST));
            DL_UART_Main_transmitData(UART_0_INST, '\n');
 
            /* CRC line */
            len = snprintf(buf, sizeof(buf),
                           "Checksum:      %08lX\r\n",
                           (unsigned long)entry.crc32);
            for (int i = 0; i < len; i++) {
                while (DL_UART_isBusy(UART_0_INST));
                DL_UART_Main_transmitData(UART_0_INST, (uint8_t)buf[i]);
            }
 
            /* Also print a plain AQI reading for human monitoring (FT-03) */
            uint32_t final_aqi = Map_RawADC_to_AQI(aqiAverage);
            len = snprintf(buf, sizeof(buf),
                           "AQI: %lu\r\n---\r\n",
                           (unsigned long)final_aqi);
            for (int i = 0; i < len; i++) {
                while (DL_UART_isBusy(UART_0_INST));
                DL_UART_Main_transmitData(UART_0_INST, (uint8_t)buf[i]);
            }
        }
    }
}
 
/* -----------------------------------------------------------------------
 * DMA_IRQHandler  (HARDWARE BUILD — replace vSimulatorTask with this)
 *
 * Called automatically by hardware when the DMA finishes transferring 10
 * ADC samples into gADCBuffer.  Gives xSamplingSemaphore from ISR context
 * to wake the Processing Task.
 *
 * Steps to enable:
 *   1. Remove vSimulatorTask from main() below.
 *   2. Uncomment this handler.
 *   3. Ensure the DMA channel and interrupt are configured in your .syscfg.
 * ----------------------------------------------------------------------- */
/*
void DMA_IRQHandler(void)
{
    DL_DMA_clearInterruptStatus(DMA, DL_DMA_EVENT_IIDX_DMACH0);
 
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xSemaphoreGiveFromISR(xSamplingSemaphore, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}
*/
 
/* -----------------------------------------------------------------------
 * main
 * ----------------------------------------------------------------------- */
int main(void)
{
    SYSCFG_DL_init();
 
    /* ---- Create inter-task communication objects ----------------------- */
 
    /* Binary semaphore: DMA ISR (or simulator) → Processing Task */
    xSamplingSemaphore = xSemaphoreCreateBinary();
 
    /* AQI queue: Processing Task → Logging Task (depth 4, every 60 s) */
    xAQIQueue  = xQueueCreate(4,  sizeof(uint32_t));
 
    /* UART queue: Logging Task → UART Task (depth 4) */
    xUARTQueue = xQueueCreate(4,  sizeof(AQILogEntry_t));
 
    if (xSamplingSemaphore == NULL ||
        xAQIQueue          == NULL ||
        xUARTQueue         == NULL) {
        /* Allocation failed — halt */
        while (1) { __BKPT(0); }
    }
 
    /* ---- Create tasks in priority order (lowest first) ----------------- */
 
    /* Priority 1 - UART Task (lowest: never pre-empts real-time tasks) */
    xTaskCreate(vUARTTask,        "UART_CLI",   256, NULL, PRIORITY_UART,       NULL);
 
    /* Priority 2 - Logging Task */
    xTaskCreate(vLoggingTask,     "AES_Logger",  128, NULL, PRIORITY_LOGGING,    NULL);
 
    /* Priority 3 - Processing Task */
    xTaskCreate(vProcessingTask,  "ADC_Process", 128, NULL, PRIORITY_PROCESSING, NULL);
 
    /* Priority 4 - Simulator Task (REMOVE for hardware build, add DMA_IRQHandler) */
    xTaskCreate(vSimulatorTask,   "ADC_Sim",     128, NULL, PRIORITY_SIMULATOR,  NULL);
 
    vTaskStartScheduler();  /* does not return */
 
    while (1) { __BKPT(0); }
}