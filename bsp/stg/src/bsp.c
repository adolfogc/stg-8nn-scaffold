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

#include "adc.h"
#include "can.h"
#include "gpio.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"

#include "bsp.h"
#include "bsp_clock.h"
#include "bsp_pinout.h"

void BSP_init(void)
{
    HAL_Init();
    /* Configure the system clock */
    BSP_systemClockConfig();
    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_CAN_Init();
    MX_ADC_Init();
    /* Note: Watchdog can be enabled by calling MX_IWDG_Init/0 here */ 
    MX_RTC_Init();
    MX_TIM16_Init();
    MX_TIM17_Init();
    MX_USART1_UART_Init();
    MX_USART3_IRDA_Init();
    MX_I2C2_Init();
    MX_TIM1_Init();

#ifdef MODEL_STG850
    MX_TIM2_Init();
#endif /* MODEL_STG850 */

#ifdef MODEL_STG856
    MX_USART5_UART_Init();
#endif /* MODEL_STG856 */
}

void BSP_Led_off(void)
{
    LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
}

void BSP_Led_on(void)
{
    LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
}