#ifndef APPLICATION_DATA_LOGGER_H
#define APPLICATION_DATA_LOGGER_H
 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "../drivers/security_hardware.h"
 
/* -----------------------------------------------------------------------
 * Circular Buffer Capacity
 *
 * 1024 entries × 24 bytes/entry = 24,576 bytes of the 32 KB SRAM.
 * Reduce LOG_BUFFER_CAPACITY if other allocations are too large.
 * ----------------------------------------------------------------------- */
#define LOG_BUFFER_CAPACITY  16U
 
/* -----------------------------------------------------------------------
 * AQI Log Entry
 * Stored in the circular buffer and forwarded to the UART task.
 * ----------------------------------------------------------------------- */
typedef struct {
    uint8_t  encrypted[AES_BLOCK_SIZE]; /* AES-128 ciphertext              */
    uint32_t crc32;                     /* CRC-32 of encrypted[] (EX-03)   */
    uint32_t uptime_ms;                 /* xTaskGetTickCount() at log time  */
} AQILogEntry_t;
 
/* -----------------------------------------------------------------------
 * UART queue: Logging Task → UART Task
 * Queue holds pointers into gLogBuffer so no extra copies are needed.
 * The UART Task reads the entry before the circular buffer wraps around.
 * ----------------------------------------------------------------------- */
extern QueueHandle_t xUARTQueue;
 
/* -----------------------------------------------------------------------
 * Task Prototype
 * ----------------------------------------------------------------------- */
void vLoggingTask(void *pvParameters);
 
#endif /* APPLICATION_DATA_LOGGER_H */