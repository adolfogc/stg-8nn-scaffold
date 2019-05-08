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
static SocketCANInstance l_socketcan;

BSP_CAN_FilterConfig * BSP_CAN_getFilterConfig(void)
{
    /* Not implemented */
    return NULL;
}

void BSP_CAN_newMessageFilter(BSP_CAN_FilterRule* rule, const uint32_t srcNodes, const uint32_t destNodes)
{
    (void) rule;
    (void) srcNodes;
    (void) destNodes;
    /* Not implemented */
}

void BSP_CAN_newServiceFilter(BSP_CAN_FilterRule* rule, const uint32_t srcNodes, const uint32_t destNodes)
{
    (void) rule;
    (void) srcNodes;
    (void) destNodes;
    /* Not implemented */  
}

bool BSP_CAN_init(BSP_CAN_FilterConfig * const filterConfig)
{
    (void) filterConfig; /* unused parameter */

    return socketcanInit(&l_socketcan, BSP_CAN_IFACE_NAME) == 0;
}

BSP_CAN_RxTxResult BSP_CAN_transmitOnce(const CanardCANFrame* frame)
{
    const int16_t txRes = socketcanTransmit(&l_socketcan, frame, 0U);
    BSP_CAN_RxTxResult result = BSP_CAN_RXTX_ERROR;
    if (txRes > 0) {
        result = BSP_CAN_RXTX_SUCCESS;
    } else {
        if (txRes == 0) {
            result = BSP_CAN_RXTX_TIMEOUT;
        }
    }
    return result;
}

BSP_CAN_RxTxResult BSP_CAN_receiveOnce(CanardCANFrame* frame)
{
    const int16_t rxRes = socketcanReceive(&l_socketcan, frame, 100U); /* 100 ms timeout */
    BSP_CAN_RxTxResult result = BSP_CAN_RXTX_ERROR;
    if (rxRes > 0) {
        result = BSP_CAN_RXTX_SUCCESS;
    } else {
        if (rxRes == 0) {
            result = BSP_CAN_RXTX_TIMEOUT;
        }
    }
    return result;
}
