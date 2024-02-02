#include "qpc.h"

#include "stm32f0xx_hal.h"
#include "stm32f0xx_ll_utils.h"

#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "bsp_clock.h"
#include "bsp_isr_systick.h"
#include "bsp_qpc.h"
#include "bsp_specific.h"

static const uint32_t MaxUint32Value = 0xFFFFFFFFU;

static volatile uint32_t l_nTicks = 0U;
static volatile uint32_t l_upTimeSeconds = 0U;

static QTicker l_ticker0;

QActive * const AO_ticker0 = &l_ticker0.super;

void SysTick_Handler(void) { /* every 1ms */
  
  QK_ISR_ENTRY();

  /* Update tick count */
  ++l_nTicks;

  HAL_IncTick(); // needs to be called every 1ms
  
  if (l_nTicks % BSP_TICKS_PER_SEC == 0U) {
    l_nTicks = 0U;
    if (l_upTimeSeconds != MaxUint32Value) {
      ++l_upTimeSeconds;
    }
  }

  QTICKER_TRIG(AO_ticker0, (void *)0); // trigger ticker AO

  QK_ISR_EXIT();
}

uint32_t BSP_upTimeSeconds(void) { return l_upTimeSeconds; }

/* 
  NOTE: Do not use this function.
  It is only provided for use in the initialization of Libcanard's STM32 driver.
 */
void usleep(uint32_t usec);

void usleep(uint32_t usec) {
  const uint32_t msec = usec / 1000U;
  LL_mDelay(msec); // blocking delay
}
