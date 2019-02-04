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

#ifndef _STG_PINOUT_H
#define _STG_PINOUT_H

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_hal_gpio.h"

#ifdef MODEL_STG850
#define OUT1_HS_Pin LL_GPIO_PIN_13
#define OUT1_HS_GPIO_Port GPIOC
#define OUT2_HS_Pin LL_GPIO_PIN_14
#define OUT2_HS_GPIO_Port GPIOC
#define OUT3_HS_Pin LL_GPIO_PIN_15
#define OUT3_HS_GPIO_Port GPIOC
#define OUT4_HS_Pin LL_GPIO_PIN_4
#define OUT4_HS_GPIO_Port GPIOA
#define OUT5_HS_Pin LL_GPIO_PIN_7
#define OUT5_HS_GPIO_Port GPIOA
#define OUT6_HS_Pin LL_GPIO_PIN_0
#define OUT6_HS_GPIO_Port GPIOB
#define OUT7_HS_Pin LL_GPIO_PIN_1
#define OUT7_HS_GPIO_Port GPIOB
#define OUT8_HS_Pin LL_GPIO_PIN_2
#define OUT8_HS_GPIO_Port GPIOB
#define LED_Pin LL_GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define CAN_S_Pin LL_GPIO_PIN_6
#define CAN_RX_Pin LL_GPIO_PIN_8
#define CAN_TX_Pin LL_GPIO_PIN_9
#define CAN_GPIO_Port GPIOB
#define Out9_LS_Pin LL_GPIO_PIN_7
#define Out9_LS_GPIO_Port GPIOB
#define BOOT0_Pin LL_GPIO_PIN_11
#define BOOT0_GPIO_Port GPIOF
#endif /* MODEL_STG850 */

#ifdef MODEL_STG856
#define OUT1_Pin GPIO_PIN_13
#define OUT1_GPIO_Port GPIOC
#define OUT2_Pin GPIO_PIN_14
#define OUT2_GPIO_Port GPIOC
#define OUT3_Pin GPIO_PIN_15
#define OUT3_GPIO_Port GPIOC
#define IN1_Pin GPIO_PIN_0
#define IN1_GPIO_Port GPIOA
#define IN2_Pin GPIO_PIN_1
#define IN2_GPIO_Port GPIOA
#define IN3_Pin GPIO_PIN_2
#define IN3_GPIO_Port GPIOA
#define IN4_Pin GPIO_PIN_3
#define IN4_GPIO_Port GPIOA
#define OUT4_Pin GPIO_PIN_4
#define OUT4_GPIO_Port GPIOA
#define IN5_Pin GPIO_PIN_5
#define IN5_GPIO_Port GPIOA
#define IN6_Pin GPIO_PIN_6
#define IN6_GPIO_Port GPIOA
#define OUT5_Pin GPIO_PIN_7
#define OUT5_GPIO_Port GPIOA
#define OUT6_Pin GPIO_PIN_0
#define OUT6_GPIO_Port GPIOB
#define OUT7_Pin GPIO_PIN_1
#define OUT7_GPIO_Port GPIOB
#define OUT8_Pin GPIO_PIN_2
#define OUT8_GPIO_Port GPIOB
#define UART_ON_Pin GPIO_PIN_12
#define UART_ON_GPIO_Port GPIOB
#define DRIVER_EN_Pin GPIO_PIN_15
#define DRIVER_EN_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define IN9_Pin GPIO_PIN_11
#define IN9_GPIO_Port GPIOA
#define IN10_Pin GPIO_PIN_12
#define IN10_GPIO_Port GPIOA
#define IN8_Pin GPIO_PIN_15
#define IN8_GPIO_Port GPIOA
#define IN7_Pin GPIO_PIN_5
#define IN7_GPIO_Port GPIOB
#define CAN_S_Pin GPIO_PIN_6
#define CAN_RX_Pin LL_GPIO_PIN_8
#define CAN_TX_Pin LL_GPIO_PIN_9
#define CAN_GPIO_Port GPIOB
#define OUT9_Pin GPIO_PIN_7
#define OUT9_GPIO_Port GPIOB
#define Boot0_Pin GPIO_PIN_11
#define Boot0_GPIO_Port GPIOF
#endif /* MODEL_STG856 */

#endif /* _STG_856_PINOUT_H */