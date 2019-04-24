#include "stm32f0xx_hal.h"

#include "bsp.h"
#include "bsp_clock.h"
#include "bsp_qpc.h"
#include "bsp_specific.h"
#include "bsp_isr_systick.h"

Q_DEFINE_THIS_FILE

#define MAX_UINT32_VALUE 0xFFFFFFFFU

static uint32_t volatile l_nTicks = 0U;
static uint32_t volatile l_upTimeSeconds = 0U;

static volatile QTicker * l_ticker0Ptr = NULL;
static volatile QTicker l_ticker0;

void SysTick_Handler(void)
{
  QK_ISR_ENTRY();

  /* Update tick count */
  ++l_nTicks;
  if(l_nTicks % BSP_TICKS_PER_MS == 0U) {
      HAL_IncTick();
  }
  if(l_nTicks % BSP_TICKS_PER_SEC == 0U) {
      l_nTicks = 0U;
      if(l_upTimeSeconds != MAX_UINT32_VALUE) {
        ++l_upTimeSeconds;
      }
  }

  if(l_ticker0Ptr != NULL) {
    QACTIVE_POST((QTicker*)l_ticker0Ptr, 0U, (void *)0);
  } else {
    QF_TICK_X(0U, (void *)0);
  }

  QK_ISR_EXIT();
}

void BSP_Ticker0_initAO(void)
{
    QTicker_ctor((QTicker*)&l_ticker0, 0U);
    l_ticker0Ptr = &l_ticker0;
}

void BSP_Ticker0_startAO(uint8_t priority)
{
    QACTIVE_START((QActive*)&l_ticker0,
      priority,
      (void*)0,
      0U,
      (void*)0, 0U,
      (QEvt*)0);
}

QTicker* BSP_Ticker0_getAO(void)
{
    return (QTicker*)&l_ticker0;
}


uint32_t BSP_upTimeSeconds(void)
{
    return l_upTimeSeconds;
}

/* IMPORTANT: Do not use this function. It is only provided for use in the initialization of Libcanard's STM32 driver. */
void usleep(uint32_t usec);

void usleep(uint32_t usec)
{
    const uint32_t startTicks = l_nTicks;
    const uint32_t delayTicks = (usec / 100U) * BSP_TICKS_PER_100uS + 1; /* add 1 to guarantee minimum delay */
    while(l_nTicks - startTicks < delayTicks) {
        /* Busy waiting... */
    }
}
