/*
Copyright (C) 2019 Adolfo E. García

This file is part of STG-8nn-Scaffold.

STG-8nn-Scaffold is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

STG-8nn-Scaffold is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "stm32f0xx_hal.h"

#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_utils.h"

#include "bsp.h"
#include "bsp_clock.h"
#include "bsp_isr_priorities.h"
#include "bsp_qpc.h"

Q_DEFINE_THIS_FILE

void BSP_systemClockConfig(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /* Initialize the CPU, AHB and APB buses' clocks */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14 | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16U;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  Q_ENSURE(HAL_RCC_OscConfig(&RCC_OscInitStruct) == HAL_OK);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  Q_ENSURE(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) == HAL_OK);

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3 | RCC_PERIPHCLK_USART1 | RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  Q_ENSURE(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) == HAL_OK);

  /* Configure the Systick interrupt
   * Note 0: The function HAL_SYSTICK_Config/1 already substracts one from the reload value, so don't substract one.
   * Note 1: The HAL drivers require that the systick interrupt occurrs every 1 ms.
   */
  const uint32_t u32Reload = HAL_RCC_GetHCLKFreq() / BSP_TICKS_PER_SEC;
  HAL_SYSTICK_Config(u32Reload);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, BSP_SYSTICK_PRIO, 0);
}

/* Alternative implementation: work in progress */
void BSP_systemClockConfig2(void);

void BSP_systemClockConfig2(void) {
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
  Q_ENSURE(LL_FLASH_GetLatency() == LL_FLASH_LATENCY_0);

  LL_RCC_HSE_Enable();
  while (LL_RCC_HSE_IsReady() != 1) {
    /* Wait till HSE is ready */
  }

  LL_RCC_HSI14_Enable();
  while (LL_RCC_HSI14_IsReady() != 1) {
    /* Wait till HSI14 is ready */
  }
  LL_RCC_HSI14_SetCalibTrimming(16);

  LL_RCC_LSI_Enable();
  while (LL_RCC_LSI_IsReady() != 1) {
    /* Wait till LSI is ready */
  }

  LL_PWR_EnableBkUpAccess();
  LL_RCC_ForceBackupDomainReset();
  LL_RCC_ReleaseBackupDomainReset();
  LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_HSE_DIV32);
  LL_RCC_EnableRTC();
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);

  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE) {
    /* Wait till System clock is ready */
  }

  LL_Init1msTick(16000000);
  LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
  LL_SetSystemCoreClock(16000000);
  LL_RCC_HSI14_EnableADCControl();
  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
  LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);

  /* SysTick_IRQn interrupt configuration */
  NVIC_SetPriority(SysTick_IRQn, BSP_SYSTICK_PRIO);
}
