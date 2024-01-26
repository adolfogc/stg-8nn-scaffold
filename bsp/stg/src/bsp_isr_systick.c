#include "qpc.h"

#include "stm32f0xx_hal.h"

#include <stdbool.h>
#include <stdint.h>

#include "bsp.h"
#include "bsp_clock.h"
#include "bsp_isr_systick.h"
#include "bsp_qpc.h"
#include "bsp_specific.h"

Q_DEFINE_THIS_FILE

static const uint32_t MaxUint32Value = 0xFFFFFFFFU;

static volatile uint32_t l_nTicks = 0U;
static volatile uint32_t l_upTimeSeconds = 0U;

static QTicker volatile l_ticker0;
static bool volatile ticker0IsInitialized = false;

void SysTick_Handler(void) {
  
  QK_ISR_ENTRY();

  /* Update tick count */
  ++l_nTicks;

  HAL_IncTick();
  
  if (l_nTicks % BSP_TICKS_PER_SEC == 0U) {
    l_nTicks = 0U;
    if (l_upTimeSeconds != MaxUint32Value) {
      ++l_upTimeSeconds;
    }
  }

  QTICKER_TRIG(&l_ticker0.super, (void *)0); // trigger ticker AO

  QK_ISR_EXIT();
}

void BSP_Ticker0_initAO(void) {
  QTicker_ctor(&l_ticker0, 0U);
  ticker0IsInitialized = true;
}

void BSP_Ticker0_startAO(uint8_t priority) {
  QACTIVE_START(&l_ticker0.super, priority, (void *)0, 0U, (void *)0, 0U, (QEvt *)0);
}

QTicker *BSP_Ticker0_getAO(void) { return &l_ticker0; }

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
