/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

#include "stm32f0xx.h"
#include "stm32f0xx_hal.h"
#include "stm32f0xx_it.h"
#include "can.h"
#include "bsp_clock.h"
#include "bsp_qpc.h"

/* -- Cortex-M0 Processor Interruption and Exception Handlers -- */ 

/* NMI_Handler/0 and PendSV_Handler/0 are implemented in QP's kernel port */

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
  BSP_updateTickCounts();
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
  QK_ISR_EXIT();
}

void CEC_CAN_IRQHandler(void)
{
  QK_ISR_ENTRY();
  QK_ISR_EXIT();
}