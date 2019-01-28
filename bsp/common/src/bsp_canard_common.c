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

#include "bsp.h"
#include "bsp_canard_common.h"
#include "bsp_uavcan_constants.h"

#include <string.h>

/* -- Library instance -- */
CanardInstance g_canard;

/* -- Canard callbacks -- */
bool BSP_Canard_shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
    if ((transferType == CanardTransferTypeRequest) && (dataTypeId == UAVCAN_GET_NODE_INFO_DATA_TYPE_ID)) {
        *outDataTypeSignature = UAVCAN_GET_NODE_INFO_DATA_TYPE_SIGNATURE;
        return true;
    }
    return false;
}

void BSP_Canard_onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer)
{
    if ((transfer->transfer_type == CanardTransferTypeRequest) && (transfer->data_type_id == UAVCAN_GET_NODE_INFO_DATA_TYPE_ID)) {
        BSP_Canard_getNodeInfoHandle(transfer);
    }
}

/* -- Helper functions -- */
void BSP_Canard_makeNodeStatusMessage(uint8_t* buffer)
{
    const uint8_t node_health = UAVCAN_NODE_HEALTH_OK;
    const uint8_t node_mode   = UAVCAN_NODE_MODE_OPERATIONAL;
    memset(buffer, 0, UAVCAN_NODE_STATUS_MESSAGE_SIZE);
    const uint32_t uptime_sec = BSP_upTimeSeconds();
    canardEncodeScalar(buffer,  0, 32, &uptime_sec);
    canardEncodeScalar(buffer, 32,  2, &node_health);
    canardEncodeScalar(buffer, 34,  3, &node_mode);
}

uint16_t BSP_Canard_makeNodeInfoMessage(uint8_t* buffer)
{
    memset(buffer, 0, UAVCAN_GET_NODE_INFO_RESPONSE_MAX_SIZE);
    BSP_Canard_makeNodeStatusMessage(buffer);
 
    buffer[7] = APP_VERSION_MAJOR;
    buffer[8] = APP_VERSION_MINOR;
    buffer[9] = 1;  /* Optional field flags, Git commit is set */
    const uint32_t gitHash_ = APP_VERSION_GIT_COMMIT_HASH;
    canardEncodeScalar(buffer, 80, 32, &gitHash_);
 
    BSP_readUniqueID(&buffer[24]);
    const size_t nameLen = strlen(APP_NODE_NAME);
    memcpy(&buffer[41], APP_NODE_NAME, nameLen);
    return 41 + nameLen ;
}

void BSP_Canard_getNodeInfoHandle(CanardRxTransfer* transfer)
{
    uint8_t buffer[UAVCAN_GET_NODE_INFO_RESPONSE_MAX_SIZE];
    memset(buffer, 0, UAVCAN_GET_NODE_INFO_RESPONSE_MAX_SIZE);
    const uint16_t len = BSP_Canard_makeNodeInfoMessage(buffer);
    int result = canardRequestOrRespond(&g_canard,
                                        transfer->source_node_id,
                                        UAVCAN_GET_NODE_INFO_DATA_TYPE_SIGNATURE,
                                        UAVCAN_GET_NODE_INFO_DATA_TYPE_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        &buffer[0],
                                        (uint16_t)len);
    if (result < 0)
    {
        // TODO: handle the error
    }
}

/* -- TODO: replace -- */
void BSP_readUniqueID(uint8_t* outUid)
{
    /* FIXME: substitute mock unique ID */
    const char * const uid_ = "0123456789ABCDEF";
    for (uint8_t i = 0; i < UNIQUE_ID_LENGTH_BYTES; ++i) {
        outUid[i] = (uint8_t)uid_[i];
    }
}