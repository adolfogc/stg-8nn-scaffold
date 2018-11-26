#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"
#include "bsp/bsp_qpc.h"

extern CAN_HandleTypeDef hcan;
extern IRDA_HandleTypeDef hirda3;

/* -- Cortex-M0 Processor Interruption and Exception Handlers -- */ 

/* This function handles Hard fault interrupt. */
void HardFault_Handler(void)
{
  while (1)
  {
    /* Do nothing */
  }
}

/* This function handles System service call via SWI instruction. */
void SVC_Handler(void)
{
  QK_ISR_ENTRY();
  /* Not implemented */
  QK_ISR_EXIT();
}

/* This function handles System tick timer. */
void SysTick_Handler(void)
{
  QK_ISR_ENTRY();
  QF_TICK_X(0U, (void *)0);
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
  QK_ISR_EXIT();
}

/* 
 * STM32F0xx Peripheral Interrupt Handlers
 * Add here the Interrupt Handlers for the used peripherals.
 * For the available peripheral interrupt handler names,
 * please refer to the startup file (startup_stm32f0xx.s).
 */

/* This function handles USART3 to USART8 global interrupts / USART3 wake-up interrupt through EXTI line 28. */
void USART3_8_IRQHandler(void)
{
  QK_ISR_ENTRY();
  HAL_IRDA_IRQHandler(&hirda3);
  QK_ISR_EXIT();
}

/* This function handles HDMI-CEC and CAN global interrupts / HDMI-CEC wake-up interrupt through EXTI line 27. */
void CEC_CAN_IRQHandler(void)
{
  QK_ISR_ENTRY();
  HAL_CAN_IRQHandler(&hcan);
  QK_ISR_EXIT();
}