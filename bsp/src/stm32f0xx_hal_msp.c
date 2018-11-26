#include "stm32f0xx_hal.h"
#include "bsp/irs_pri.h"

/* Initializes the Global MSP. */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* -- System interrupt init -- */
  /* SVC_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SVC_IRQn, BSP_SVC_PRIO, 0);
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, BSP_SYSTICK_PRIO, 0);
}