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

static void statusUpdate(void)
{
    static uint8_t transferId = 0; /* The transferId variable MUST BE STATIC; refer to the libcanard documentation for the explanation. */

    uint8_t messageBuffer[UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE];
    memset(messageBuffer, 0, UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE);

    const uint32_t len = makeNodeStatusMessage(messageBuffer);

    canardBroadcast(&l_canardInstance,
                    UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE,
                    UAVCAN_PROTOCOL_NODESTATUS_ID,
                    &transferId,
                    CANARD_TRANSFER_PRIORITY_LOW,
                    messageBuffer,
                    (uint16_t)len);
}

static void restartNodeHandle(CanardRxTransfer* transfer)
{
    uint8_t buffer[UAVCAN_PROTOCOL_RESTARTNODE_REQUEST_MAX_SIZE];

    uavcan_protocol_RestartNodeRequest restartNodeRequest;
    uavcan_protocol_RestartNodeRequest_decode(transfer, transfer->payload_len, &restartNodeRequest, (uint8_t**)0);

    /* Abort if magic number is incorrect */
    if(restartNodeRequest.magic_number != UAVCAN_PROTOCOL_RESTARTNODE_REQUEST_MAGIC_NUMBER) {
        return;
    }

    uavcan_protocol_RestartNodeResponse restartNodeResponse;
    restartNodeResponse.ok = true;

    const uint32_t len = uavcan_protocol_RestartNodeResponse_encode(&restartNodeResponse, buffer);

    int result = canardRequestOrRespond(&l_canardInstance,
                                        transfer->source_node_id,
                                        UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE,
                                        UAVCAN_PROTOCOL_RESTARTNODE_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        buffer,
                                        (uint16_t)len);
    if (result >= 0) {
        static const QEvt aboutToRestartEvt = {UAVCAN_RESTART_SIG, 0U, 0U};
        QACTIVE_POST(&l_uavcanNode.super, &aboutToRestartEvt, (void*)0);
    }
}

static void getNodeInfoHandle(CanardRxTransfer* transfer)
{
    uint8_t messageBuffer[UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH];
    memset(messageBuffer, 0, UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH);

    const uint32_t len = makeNodeInfoMessage(messageBuffer);

    int result = canardRequestOrRespond(&l_canardInstance,
                                        transfer->source_node_id,
                                        UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
                                        UAVCAN_PROTOCOL_GETNODEINFO_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        messageBuffer,
                                        (uint16_t)len);
    (void) result; /* Unused */
}

static uint32_t makeNodeStatusMessage(uint8_t* messageBuffer)
{
    uavcan_protocol_NodeStatus nodeStatus;
    memset(&nodeStatus, 0, sizeof(nodeStatus));

    nodeStatus.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    nodeStatus.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    nodeStatus.uptime_sec = BSP_upTimeSeconds();

    return uavcan_protocol_NodeStatus_encode(&nodeStatus, messageBuffer);
}

static uint32_t makeNodeInfoMessage(uint8_t* messageBuffer)
{
    uavcan_protocol_GetNodeInfoResponse nodeInfoResponse;
    memset(&nodeInfoResponse, 0, sizeof(nodeInfoResponse));

    nodeInfoResponse.name.data = (uint8_t*) APP_UAVCAN_NODE_NAME_DATA;
    nodeInfoResponse.name.len = APP_UAVCAN_NODE_NAME_LEN;

    nodeInfoResponse.status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    nodeInfoResponse.status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    nodeInfoResponse.status.uptime_sec = BSP_upTimeSeconds();

    nodeInfoResponse.software_version.major = APP_SW_VERSION_MAJOR;
    nodeInfoResponse.software_version.minor = APP_SW_VERSION_MINOR;
    nodeInfoResponse.software_version.optional_field_flags = 1;
    nodeInfoResponse.software_version.vcs_commit = APP_SW_GIT_COMMIT_HASH; /* DEFINED BY CMAKE */

    nodeInfoResponse.hardware_version.major = APP_HW_VERSION_MAJOR;
    nodeInfoResponse.hardware_version.minor = APP_HW_VERSION_MINOR;

    BSP_readUniqueID(&(nodeInfoResponse.hardware_version.unique_id[0]));   /* Writes unique ID into the buffer */

    return uavcan_protocol_GetNodeInfoResponse_encode(&nodeInfoResponse, messageBuffer);
}
