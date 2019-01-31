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

Q_DEFINE_THIS_FILE

#define BSP_CAN_IFACE_NAME "vcan0"

/* SocketCan driver instance */
static SocketCANInstance g_socketcan;

void BSP_CAN_init(void)
{
    Q_ENSURE(socketcanInit(&g_socketcan, BSP_CAN_IFACE_NAME) == 0);
}

void BSP_CAN_sendAll(CanardInstance* canardInstance)
{
    const CanardCANFrame* txFrame = canardPeekTxQueue(canardInstance);
    while(txFrame != NULL)
    {
        const int16_t txRes = socketcanTransmit(&g_socketcan, txFrame, 0U);
        if (txRes < 0) {

            /* FIXME: handle the error properly */
        } else if(txRes > 0) {
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
    const int16_t rxRes = socketcanReceive(&g_socketcan, &rxFrame, 0U); /* FIXME: timeout */
    if (rxRes < 0) {
        /* Failure */
        (void)fprintf(stderr, "Receive error %d, errno '%s'\n", rxRes, strerror(errno));
    }
    else if (rxRes > 0) {
        /* Success */
        canardHandleRxFrame(canardInstance, &rxFrame, upTime);
    } else {
        /* Timeout */
        fputs("Timed out while receiving CAN frame.\n", stderr);
    }
}