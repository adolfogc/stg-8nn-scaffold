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
#include "drivers/stm32/canard_stm32.h"
#include "bsp.h"
#include "bsp_qpc.h"

Q_DEFINE_THIS_FILE

void BSP_CAN_init(void)
{
    CanardSTM32CANTimings timings;

    /* Change CAN from silent to active mode. */
    LL_GPIO_ResetOutputPin(CAN_S_GPIO_Port, CAN_S_Pin);

    /* Initialize the STM32 CAN driver */
    Q_ENSURE(canardSTM32ComputeCANTimings(HAL_RCC_GetPCLK1Freq(), 500000U, &timings) == 0U);
    Q_ENSURE(canardSTM32Init(&timings, CanardSTM32IfaceModeNormal) == 0U);
}

void BSP_CAN_sendAll(CanardInstance* canardInstance)
{
    const CanardCANFrame* txFrame = canardPeekTxQueue(canardInstance);
    while(txFrame != NULL)
    {
        const int16_t txRes = canardSTM32Transmit(txFrame);
        if (txRes < 0) { /* Failure - drop the frame and report */
            /* FIXME: handle the error properly */
        } else if(txRes == 0) {
            /* FIXME: handle full TX queue */
        } else if(txRes == 1) {
            canardPopTxQueue(canardInstance);
        } else {
            /* Do nothing */
        }
        txFrame = canardPeekTxQueue(canardInstance);
    }
}

void BSP_CAN_receiveOnce(CanardInstance* canardInstance)
{
    CanardCANFrame rxFrame;
    const uint32_t upTime = BSP_upTimeSeconds();
    const int16_t rxRes = canardSTM32Receive(&rxFrame);
    if (rxRes == 0) {
        /* FIXME: handle empty buffer */
    } else if(rxRes == 1) {
        canardHandleRxFrame(canardInstance, &rxFrame, upTime);
    } else {
        /* FIXME: handle error */
    }
}


