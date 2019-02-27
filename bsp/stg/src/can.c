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

#include "stm32f0xx_ll_gpio.h"
#include "stm32f0xx_ll_bus.h"
#include "bsp_isr_priorities.h"
#include "bsp_pinout.h"
#include "bsp_qpc.h"
#include "can.h"

/* CAN init function */
void MX_CAN_Init(void)
{
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_CAN);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    /* Set CAN to silent mode */
    LL_GPIO_SetOutputPin(CAN_GPIO_Port, CAN_S_Pin);

    GPIO_InitStruct.Pin = CAN_S_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(CAN_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = CAN_RX_Pin|CAN_TX_Pin;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
    LL_GPIO_Init(CAN_GPIO_Port, &GPIO_InitStruct);

    NVIC_SetPriority(CEC_CAN_IRQn, BSP_CAN_PRIO);
    /* The IRQ can be enabled by calling: NVIC_EnableIRQ(CEC_CAN_IRQn) */
}
