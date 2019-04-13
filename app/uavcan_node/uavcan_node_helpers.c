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

static void initCanardInstance(void)
{
    canardInit(
        &l_canardInstance,
        l_canardMemoryPool,
        APP_CANARD_MEMORY_POOL_SIZE,
        onTransferReceived,
        shouldAcceptTransfer,
        NULL
    );

    canardSetLocalNodeID(&l_canardInstance, APP_UAVCAN_DEFAULT_NODE_ID);
}

static void sendAll(void)
{
    while(canardPeekTxQueue(&l_canardInstance) != NULL) {
        const BSP_CAN_RxTxResult result = BSP_CAN_transmitOnce(canardPeekTxQueue(&l_canardInstance));
        switch (result) {
            case BSP_CAN_RXTX_TIMEOUT:
                /* TODO: handle case */
                break;
            case BSP_CAN_RXTX_SUCCESS:
                canardPopTxQueue(&l_canardInstance);
                break;
            case BSP_CAN_RXTX_ERROR:
                /* TODO: handle case */
                break;
            default:
                /* This path should be unreacheable */
                break;
        }
    }
}

static void receiveAll(void)
{
    CanardCANFrame rxFrame;
    bool timedOut = false;

    while(!timedOut) {
        switch (BSP_CAN_receiveOnce(&rxFrame)) {
            case BSP_CAN_RXTX_TIMEOUT:
                /* No more frames are available */
                timedOut = true;
                break;
            case BSP_CAN_RXTX_SUCCESS:
                canardHandleRxFrame(&l_canardInstance, &rxFrame, BSP_upTimeSeconds());
                break;
            case BSP_CAN_RXTX_ERROR:
                /* TODO: handle case */
                break;
            default:
                /* This path should be unreacheable */
                break;
        }
    }
}
