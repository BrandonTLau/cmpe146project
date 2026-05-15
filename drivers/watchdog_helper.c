#include "watchdog_helper.h"
#include "ti_msp_dl_config.h"
 
/*
 * WWDT_Feed
 *
 * Calls the TI DriverLib restart function to reset the windowed watchdog
 * counter before it reaches the reset threshold.
 *
 * On the LP-MSPM0G3507, the watchdog instance name "WWDT0_INST" is generated
 * by SysConfig.  If your .syscfg names it differently, update the argument.
 *
 * The "windowed" constraint means this call must not arrive too early either;
 * only call it after the DMA semaphore is taken (i.e. once per 10 ms cycle).
 */
void WWDT_Feed(void)
{
    DL_WWDT_restart(WWDT0_INST);
}
 