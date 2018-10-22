#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/rcc.h>
#include <qpc.h>
#include "bsp.h"

void sys_tick_handler(void) {
  QK_ISR_ENTRY();
  QF_TICK_X(0U, (void *)0);
  QK_ISR_EXIT();
}

/* In Cortex-M0, all are KernelAwareISRs. */
enum KernelAwareISRs {
  SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI,
  /* ... */
  MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};

/* In Cortex-M0, there are 4 priority levels only, given by the upper two bits
* of the priority byte. No grouping available.
* "kernel-aware" interrupts should not overlap the PendSV priority
*/
//Q_ASSERT_COMPILE(MAX_KERNEL_AWARE_CMSIS_PRI <= (0xFF >>(8-__NVIC_PRIO_BITS)));

void QF_onStartup(void) {
  /* Set up the system clock */
  rcc_clock_setup_in_hsi48_out_48mhz();
  rcc_osc_ready_int_clear(RCC_HSI48);
  rcc_osc_ready_int_enable(RCC_HSI48);

  /* Set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate
  *
  * We want the systick to fire every 10 ms (BSP_TICKS_PER_SEC = 100)
  * => reload = ms * clockFrequency - 1 = 10 * 10^-3 * 48 * 10^6 - 1 = 479999
  *
  * Useful references here:
  * - https://www.youtube.com/watch?v=aLCUDv_fgoU
  * - https://www.youtube.com/watch?v=jP1JymlHUtc
  * - STM32F0xxx Cortex-M0 programming manual
  */
  systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
  systick_set_reload(479999);
  systick_clear();
  systick_counter_enable();
  systick_interrupt_enable();

  /* set priorities of ALL ISRs used in the system, see NOTE00 below.
  *
  * !!!!!!!!!!!!!!!!!!!!!!!!!!!! CAUTION !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  * Assign a priority to EVERY ISR explicitly by calling nvic_set_priority().
  * DO NOT LEAVE THE ISR PRIORITIES AT THE DEFAULT VALUE!
  */
  nvic_set_priority(NVIC_SYSTICK_IRQ, SYSTICK_PRIO);
  //nvic_set_priority(NVIC_PENDSV_IRQ, PENDSV_PRIO);

  nvic_enable_irq(NVIC_SYSTICK_IRQ);
}

void QF_onCleanup(void) {
}

void QK_onIdle(void) {
}

void Q_onAssert(char const *module, int loc) {
    /*
    * NOTE: add here your application-specific error handling
    */
    (void)module;
    (void)loc;
    QS_ASSERTION(module, loc, (uint32_t)10000U); /* report assertion to QS */
    reset_handler();
}

/*****************************************************************************
* NOTE00:
* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
* ISR priority that is disabled by the QF framework. The value is suitable
* for the NVIC_SetPriority() CMSIS function.
*
* Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
* with the numerical values of priorities equal or higher than
* QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
* are "QF-aware".
*/
