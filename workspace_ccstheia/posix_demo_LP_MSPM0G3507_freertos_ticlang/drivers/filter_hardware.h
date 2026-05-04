#ifndef DRIVERS_FILTER_HARDWARE_H
#define DRIVERS_FILTER_HARDWARE_H

#include <stdint.h>

/**
 * @brief  Calculates the average of raw ADC samples.
 * * This function is designed to work with the MathACL or CPU to filter
 * noise from the AQI sensor data.
 * * @param  buffer Pointer to the array of uint16_t samples.
 * @param  size   The number of samples in the buffer.
 * @return uint32_t The calculated average.
 */
uint32_t MathACL_CalculateAverage(uint16_t *buffer, uint16_t size);

#endif /* DRIVERS_FILTER_HARDWARE_H */