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

#include "bsp_clock.h"
#include "bsp.h"

#include "stm32f0xx_ll_cortex.h"
#include "stm32f0xx_ll_pwr.h"
#include "stm32f0xx_ll_rcc.h"
#include "stm32f0xx_ll_system.h"
#include "stm32f0xx_ll_utils.h"

Q_DEFINE_THIS_FILE

void _BSP_systemClockConfig(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_0);
    Q_ENSURE(LL_FLASH_GetLatency() == LL_FLASH_LATENCY_0);
    LL_RCC_HSE_Enable();
    /* Wait till HSE is ready */
    while(LL_RCC_HSE_IsReady() != 1)
    {
        /* busy waiting */
    }
    LL_RCC_HSI14_Enable();
    /* Wait till HSI14 is ready */
    while(LL_RCC_HSI14_IsReady() != 1)
    {
        /* busy waiting */
    }
    LL_RCC_HSI14_SetCalibTrimming(16);
    LL_RCC_LSI_Enable();
   /* Wait till LSI is ready */
    while(LL_RCC_LSI_IsReady() != 1)
    {
        /* busy waiting */
    }
    LL_PWR_EnableBkUpAccess();
    LL_RCC_ForceBackupDomainReset();
    LL_RCC_ReleaseBackupDomainReset();
    LL_RCC_SetRTCClockSource(LL_RCC_RTC_CLKSOURCE_HSE_DIV32);
    LL_RCC_EnableRTC();
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
    /* Wait till System clock is ready */
    while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
    {
        /* busy waiting */
    }
    LL_InitTick(16000000, BSP_TICKS_PER_SEC); /* LL_InitTick/2 already substracts one from the reload value */
    LL_SYSTICK_SetClkSource(LL_SYSTICK_CLKSOURCE_HCLK);
    LL_SetSystemCoreClock(16000000);
    LL_RCC_HSI14_EnableADCControl();
    LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);
    LL_RCC_SetUSARTClockSource(LL_RCC_USART3_CLKSOURCE_PCLK1);
    /* SysTick_IRQn interrupt configuration */
    NVIC_SetPriority(SysTick_IRQn, BSP_SYSTICK_PRIO);
}