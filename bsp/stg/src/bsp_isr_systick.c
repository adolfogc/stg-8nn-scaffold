#include "bsp_qpc.h"
#include "bsp_clock.h"
#include "bsp_isr_systick.h"

void SysTick_Handler(void)
{
  QK_ISR_ENTRY();
  QF_TICK_X(0U, (void *)0);
  BSP_updateTickCounts();
  QK_ISR_EXIT();
}
