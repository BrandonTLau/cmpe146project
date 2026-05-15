#ifndef DRIVERS_WATCHDOG_HELPER_H
#define DRIVERS_WATCHDOG_HELPER_H
 
/*
 * WWDT_Feed  -  Service the Windowed Watchdog Timer.
 *
 * Must be called from the Processing Task each time it wakes from the DMA
 * semaphore.  If the processing loop hangs and WWDT_Feed() is not called
 * within the watchdog window, the WWDT hardware triggers a system reset
 * (test EX-01).
 */
void WWDT_Feed(void);
 
#endif /* DRIVERS_WATCHDOG_HELPER_H */
 