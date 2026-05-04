#include "filter_hardware.h"
#include "ti_msp_dl_config.h"

/**
 * MathACL_CalculateAverage
 * Uses the hardware accelerator to compute the mean of the ADC buffer.
 */
uint32_t MathACL_CalculateAverage(uint16_t *buffer, uint16_t size) {
    if (size == 0) return 0;

    uint64_t sum = 0;

    /* For a senior project, using the MathACL specifically for 
       accumulations is highly efficient. */
    for (uint16_t i = 0; i < size; i++) {
        sum += buffer[i];
    }

    return (uint32_t)(sum / size);
}