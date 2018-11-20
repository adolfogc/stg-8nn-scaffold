/*
* Copyright (C) 2005-2018 Quantum Leaps, LLC. All rights reserved.
* Copyright (C) 2018 Adolfo E. García
*
* This file is part of STG-8nn-Scaffold.
*
* STG-8nn-Scaffold is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* STG-8nn-Scaffold is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with STG-8nn-Scaffold.  If not, see <www.gnu.org/licenses/>.
*/

#include <stm32f0xx_hal.h>
#include <qpc.h>
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Internal prototypes */
void SysTick_Handler(void);
void SystemClock_Config(void);

/* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
 * ISR priority that is disabled by the QF framework. The value is suitable
 * for the NVIC_SetPriority() CMSIS function.
 *
 * Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
 * with the numerical values of priorities equal or higher than
 * QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
 * are "QF-aware".
*/

/* In Cortex-M0, there are 4 priority levels only, given by the upper two bits
 * of the priority byte. No grouping available. All ISRs are KernelAware.
*/
enum KernelAwareISRs {
  SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI,
  /* ... */
  MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};

void SysTick_Handler(void) {
  QK_ISR_ENTRY();
  QF_TICK_X(0U, (void *)0);
  QK_ISR_EXIT();
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /* Initializes the CPU, AHB and APB busses clocks */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  Q_ASSERT(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  Q_ASSERT(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  Q_ASSERT(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK);

  /* Set up the SysTick timer to fire at BSP_TICKS_PER_SEC rate */

  /* Useful references on how to calculate the reload value:
   * - www.youtube.com/watch?v=aLCUDv_fgoU
   * - www.youtube.com/watch?v=jP1JymlHUtc
  */
  const uint32_t reload = HAL_RCC_GetHCLKFreq() / BSP_TICKS_PER_SEC - 1;

  HAL_SYSTICK_Config(reload);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  /* Note: SubPriority (third argument) is given a dummy value (0), as it is ignored for Cortex-M0 devices */
  HAL_NVIC_SetPriority(SysTick_IRQn, SYSTICK_PRIO,0);
}

void BSP_init(void) {
  HAL_Init();
  SystemCoreClockUpdate();
  BSP_GPIO_init();
}


void QF_onStartup(void) {
  SystemClock_Config();
}

void QF_onCleanup(void) {
  /* Not implemented */
}

void QK_onIdle(void) {
  /* Not implemented */
}

void Q_onAssert(char const *module, int loc) {
    /* Application-specific error handling goes here */
    NVIC_SystemReset();
}
