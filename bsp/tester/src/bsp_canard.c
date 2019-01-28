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

#ifndef _GNU_SOURCE
# define _GNU_SOURCE
#endif

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "drivers/socketcan/socketcan.h"
#include "bsp.h"
#include "bsp_qpc.h"
#include "bsp_canard_common.h"
#include "bsp_uavcan_constants.h"

Q_DEFINE_THIS_FILE

#define BSP_CAN_IFACE_NAME "vcan0"
#define BSP_CANARD_POOL_SIZE 1024

static uint8_t g_canard_pool[BSP_CANARD_POOL_SIZE];

/* SocketCan driver */
static SocketCANInstance g_socketcan;

/* Library initialization routine */
void BSP_Canard_init(void)
{
    Q_ENSURE(socketcanInit(&g_socketcan, BSP_CAN_IFACE_NAME) == 0);

    canardInit(
        &g_canard,
        g_canard_pool,
        BSP_CANARD_POOL_SIZE,
        BSP_Canard_onTransferReceived,
        BSP_Canard_shouldAcceptTransfer,
        NULL
    );
    canardSetLocalNodeID(&g_canard, 100);
}

void BSP_Canard_spin(void)
{    
    uint8_t buffer_[UAVCAN_NODE_STATUS_MESSAGE_SIZE];
    static uint8_t transferId_ = 0;                          // This variable MUST BE STATIC; refer to the libcanard documentation for the background
 
    fputs("SPINNING...\n", stdout);
    BSP_Canard_makeNodeStatusMessage(buffer_);
    canardBroadcast(&g_canard,
                    UAVCAN_NODE_STATUS_DATA_TYPE_SIGNATURE,
                    UAVCAN_NODE_STATUS_DATA_TYPE_ID,
                    &transferId_,
                    CANARD_TRANSFER_PRIORITY_LOW,
                    buffer_,
                    UAVCAN_NODE_STATUS_MESSAGE_SIZE);
}

void BSP_Canard_send(void)
{
    const CanardCANFrame* txFrame_ = canardPeekTxQueue(&g_canard);
    while(txFrame_ != NULL)
    {
        const int16_t txRes_ = socketcanTransmit(&g_socketcan, txFrame_, 0U);
        if (txRes_ < 0) { /* Failure - drop the frame and report */
            /* FIXME: handle the error properly */
        } else if(txRes_ > 0) {
            canardPopTxQueue(&g_canard);
        } else {
            /* Do nothing */
        }
        txFrame_ = canardPeekTxQueue(&g_canard);
    }
}

void BSP_Canard_receive(void)
{
    CanardCANFrame rxFrame_;
    const uint32_t upTime_ = BSP_upTimeSeconds();
    const int16_t rxRes_ = socketcanReceive(&g_socketcan, &rxFrame_, 0U); /* FIXME: timeout */
    if (rxRes_ < 0) { /* Failure */
        (void)fprintf(stderr, "Receive error %d, errno '%s'\n", rxRes_, strerror(errno));
    }
    else if (rxRes_ > 0) { /* Success */
        canardHandleRxFrame(&g_canard, &rxFrame_, upTime_);
    } else { /* Timeout */
        fputs("Timed out while receiving.\n", stderr);
    }
}