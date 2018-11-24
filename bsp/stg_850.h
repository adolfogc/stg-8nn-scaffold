/*
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

#ifndef _STG_850_H
#define _STG_850_H

#define OUT1_HS_Pin GPIO_PIN_13
#define OUT1_HS_GPIO_Port GPIOC
#define OUT2_HS_Pin GPIO_PIN_14
#define OUT2_HS_GPIO_Port GPIOC
#define OUT3_HS_Pin GPIO_PIN_15
#define OUT3_HS_GPIO_Port GPIOC
#define OUT4_HS_Pin GPIO_PIN_4
#define OUT4_HS_GPIO_Port GPIOA
#define OUT5_HS_Pin GPIO_PIN_7
#define OUT5_HS_GPIO_Port GPIOA
#define OUT6_HS_Pin GPIO_PIN_0
#define OUT6_HS_GPIO_Port GPIOB
#define OUT7_HS_Pin GPIO_PIN_1
#define OUT7_HS_GPIO_Port GPIOB
#define OUT8_HS_Pin GPIO_PIN_2
#define OUT8_HS_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOA
#define CAN_S_Pin GPIO_PIN_6
#define CAN_RX_Pin GPIO_PIN_8
#define CAN_TX_Pin GPIO_PIN_9
#define CAN_S_GPIO_Port GPIOB
#define OUT9_LS_Pin GPIO_PIN_7
#define OUT9_LS_GPIO_Port GPIOB
#define Boot0_Pin GPIO_PIN_11
#define Boot0_GPIO_Port GPIOF

#define DIN7_Pin GPIO_PIN_12
#define DIN7_Port GPIOA
#define DIN8_Pin GPIO_PIN_15
#define DIN8_Port GPIOA
#define DIN9_Pin GPIO_PIN_4
#define DIN9_Port GPIOB
#define DIN10_Pin GPIO_PIN_5
#define DIN10_Port GPIOB

#define EEPROM_ADDRESS			0xA0
#define EEPROM_BUFFER_SIZE	32  /* Page size */
#define EEPROM_WRITE_TIME		5   /* Page write time in ms */
#define EEPROM_TIMEOUT			10  /* Timeout for write in ms */

#define CAN_1M 1
#define CAN_500K 2
#define CAN_250K 4
#define CAN_125K 8
#define CAN_100K 10
#define CAN_50K 20
#define ADC_IN1 1
#define ADC_IN2 2
#define ADC_IN3 3
#define ADC_IN4 4
#define ADC_IN5 6
#define ADC_IN6 7

#endif /* _STG_850_H */
