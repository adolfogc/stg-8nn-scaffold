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

#include <stm32f0xx_hal.h>
#include <qpc.h>
#include "bsp.h"

Q_DEFINE_THIS_FILE

/* Internal global variables for this module */
static volatile CAN_HandleTypeDef BSP_hcan;

/* Overrides' prototypes */
void CEC_CAN_IRQHandler(void);
void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan);

/* Implementations */
void CEC_CAN_IRQHandler(void)
{
  QK_ISR_ENTRY();
  HAL_CAN_IRQHandler((CAN_HandleTypeDef*)&BSP_hcan);
  QK_ISR_EXIT();
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef* hcan)
{
  /* Not implemented yet */
}

void BSP_CAN_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  /* Call HAL's CAN init function */
  BSP_hcan.Instance = CAN;
  BSP_hcan.Init.Prescaler = 4;
  BSP_hcan.Init.Mode = CAN_MODE_NORMAL;
  BSP_hcan.Init.SJW = CAN_SJW_1TQ;
  BSP_hcan.Init.BS1 = CAN_BS1_11TQ;
  BSP_hcan.Init.BS2 = CAN_BS2_4TQ;
  BSP_hcan.Init.TTCM = DISABLE;
  BSP_hcan.Init.ABOM = ENABLE;
  BSP_hcan.Init.AWUM = DISABLE;
  BSP_hcan.Init.NART = DISABLE;
  BSP_hcan.Init.RFLM = DISABLE;
  BSP_hcan.Init.TXFP = DISABLE;
  Q_ASSERT(HAL_CAN_Init((CAN_HandleTypeDef*)&BSP_hcan) != HAL_OK);

  /* CAN clock enable */
  __HAL_RCC_CAN1_CLK_ENABLE();

  /* CAN GPIO Configuration */
  GPIO_InitStruct.Pin = CAN_RX_Pin | CAN_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
  HAL_GPIO_Init(CAN_S_GPIO_Port, &GPIO_InitStruct);
}
