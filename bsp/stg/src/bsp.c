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

#include "bsp_mx_adc.h"
#include "bsp_mx_can.h"
#include "bsp_mx_gpio.h"
#include "bsp_mx_i2c.h"
#include "bsp_mx_iwdg.h"
#include "bsp_mx_rtc.h"
#include "bsp_mx_timer.h"
#include "bsp_mx_usart.h"

#include "app.h"
#include "bsp_clock.h"

#include "uavcan/protocol/HardwareVersion.h"

#include <stdbool.h>
#include <string.h>

/* See chapter 33 - Device electronic signature of the RM0091 Reference Manual. */
uint32_t const *const Uuid = (uint32_t *)0x1FFFF7ACU;

void BSP_init(void) {
  HAL_Init();
  /* Configure the system clock */
  BSP_systemClockConfig();
  /* Initialize all configured peripherals */
  BSP_MX_GPIO_Init();
  BSP_MX_CAN_Init();
  BSP_MX_ADC_Init();
  /* Note: Watchdog can be enabled by calling MX_IWDG_Init/0 here */
  BSP_MX_RTC_Init();
  BSP_MX_TIM16_Init();
  BSP_MX_TIM17_Init();
  BSP_MX_USART1_UART_Init();
  BSP_MX_USART3_IRDA_Init();
  BSP_MX_I2C2_Init();
  BSP_MX_TIM1_Init();

#ifdef MODEL_STG850
  BSP_MX_TIM2_Init();
#endif /* MODEL_STG850 */

#ifdef MODEL_STG856
  BSP_MX_USART5_UART_Init();
#endif /* MODEL_STG856 */
}

__attribute__((noreturn)) void BSP_restart(void) { NVIC_SystemReset(); }

void BSP_Led_off(void) { LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin); }

void BSP_Led_on(void) { LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin); }

void BSP_readUniqueID(uint8_t *outUid) {
#if UAVCAN_PROTOCOL_HARDWAREVERSION_UNIQUE_ID_LENGTH != 16U
#error "UAVCAN Hardware Uuid is not 16 bytes long!"
#endif
  memcpy(&outUid[4U], Uuid, 12U);
}
