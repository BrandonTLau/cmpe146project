#include "data_logger.h"
#include "aqi_manager.h"
#include "ti_msp_dl_config.h"
#include "../drivers/security_hardware.h"
#include <string.h>
 
/* -----------------------------------------------------------------------
 * UART Queue  (defined here, extern in data_logger.h)
 * ----------------------------------------------------------------------- */
QueueHandle_t xUARTQueue = NULL;
 
/* -----------------------------------------------------------------------
 * Circular Buffer  (CC-02)
 *
 * Stores up to LOG_BUFFER_CAPACITY encrypted log entries.
 * When the write pointer reaches the end it wraps back to index 0,
 * silently overwriting the oldest record — exactly the behaviour required
 * by the memory-constraint in the design document.
 *
 * Memory: 1024 × sizeof(AQILogEntry_t) = 1024 × 24 = 24,576 bytes.
 * ----------------------------------------------------------------------- */
static AQILogEntry_t gLogBuffer[LOG_BUFFER_CAPACITY];
static uint16_t      gBufferIndex = 0;   /* next write position (0-1023)   */
 
/* -----------------------------------------------------------------------
 * vLoggingTask  (Priority 2 - MEDIUM)
 *
 * Waits for a filtered AQI value from vProcessingTask (via xAQIQueue).
 * Builds a 16-byte plaintext block, encrypts it with AES-128, computes
 * CRC-32, stores the entry in the circular buffer, then forwards it to
 * the UART task via xUARTQueue.
 * ----------------------------------------------------------------------- */
void vLoggingTask(void *pvParameters)
{
    (void)pvParameters;
 
    uint32_t receivedAQI = 0;
 
    for (;;) {
        /*
         * Block until the Processing Task sends a filtered AQI reading.
         * Timeout 1100 ms (> the 1-second heartbeat) so we can detect a
         * stalled upstream and toggle the heartbeat LED regardless.
         */
        if (xQueueReceive(xAQIQueue, &receivedAQI, pdMS_TO_TICKS(1100)) == pdPASS) {
 
            /* -- 1. Build plaintext block ----------------------------------
             * Layout (16 bytes):
             *   [0..3]  filtered AQI value       (uint32_t, big-endian)
             *   [4..7]  system uptime in ms       (uint32_t, big-endian)
             *   [8..15] zero padding
             */
            uint8_t plaintext[AES_BLOCK_SIZE];
            memset(plaintext, 0, AES_BLOCK_SIZE);
 
            uint32_t now_ms = (uint32_t)(xTaskGetTickCount() * portTICK_PERIOD_MS);
 
            plaintext[0] = (uint8_t)((receivedAQI >> 24) & 0xFF);
            plaintext[1] = (uint8_t)((receivedAQI >> 16) & 0xFF);
            plaintext[2] = (uint8_t)((receivedAQI >>  8) & 0xFF);
            plaintext[3] = (uint8_t)( receivedAQI        & 0xFF);
            plaintext[4] = (uint8_t)((now_ms     >> 24) & 0xFF);
            plaintext[5] = (uint8_t)((now_ms     >> 16) & 0xFF);
            plaintext[6] = (uint8_t)((now_ms     >>  8) & 0xFF);
            plaintext[7] = (uint8_t)( now_ms             & 0xFF);
 
            /* -- 2. AES-128 Encrypt ----------------------------------------
             * gAES_Key lives in the MPU-protected "Privileged Access Only"
             * region.  Calling this from the Logging Task (privileged) is
             * legal; the UART Task (unprivileged) cannot reach it (EX-02).
             *
             * Hardware path: replace AES128_Encrypt() body with
             *   DL_AES_setKey128() / DL_AES_loadInputData() /
             *   DL_AES_startEncryption() / DL_AES_getResult()
             */
            AQILogEntry_t entry;
            AES128_Encrypt(plaintext, gAES_Key, entry.encrypted);
 
            /* -- 3. CRC-32 over the ciphertext (EX-03) -------------------- */
            entry.crc32     = CRC32_Calculate(entry.encrypted, AES_BLOCK_SIZE);
            entry.uptime_ms = now_ms;
 
            /* -- 4. Store in circular buffer (CC-02) ----------------------- */
            gLogBuffer[gBufferIndex] = entry;
            gBufferIndex++;
            if (gBufferIndex >= LOG_BUFFER_CAPACITY) {
                gBufferIndex = 0;   /* wrap: oldest record will be overwritten */
            }
 
            /* -- 5. Forward to UART Task ----------------------------------- */
            xQueueSend(xUARTQueue, &entry, 0);
 
            /* -- 6. Rapid blink: successful secure log entry --------------- */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
 
        } else {
            /* No data for > 1.1 s — slow heartbeat to indicate idle state */
            DL_GPIO_togglePins(GPIO_LEDS_PORT, GPIO_LEDS_USER_LED_1_PIN);
        }
    }
}
 