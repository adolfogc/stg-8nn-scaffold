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

#include "uavcan/protocol/GetNodeInfo.h"
#include "uavcan/protocol/NodeStatus.h"
#include "bsp.h"
#include "app.h"
#include "uavcan_node.h"

/* -- Active Object instance -- */
static UavcanNode g_uavcanNode;

QActive * const AO_UavcanNode = &g_uavcanNode.super;

/* -- Active Object signals -- */
enum UavcanNodeSignals {
    SPIN_TIMEOUT_SIG = Q_USER_SIG,
    MAX_SIG
};

/* -- Libcanard's memory pool -- */
#define APP_CANARD_MEMORY_POOL_SIZE 1024

static uint8_t g_canardMemoryPool[APP_CANARD_MEMORY_POOL_SIZE];

/* -- Libcanard's instance -- */
CanardInstance g_canardInstance;

/* -- Prototypes for internal AO functions -- */
QState UavcanNode_initial(UavcanNode* me, QEvent const* e);
QState UavcanNode_spinning(UavcanNode* me, QEvent const* e);
QState UavcanNode_idle(UavcanNode* me, QEvent const* e);

/* -- Prototypes for internal helper functions -- */
static void spin();
static uint32_t makeNodeStatusMessage(uint8_t* buffer);
static uint32_t makeNodeInfoMessage(uint8_t* buffer);
static void getNodeInfoHandle(CanardRxTransfer* transfer);

/* -- Prototypes for Libcanard's callback -- */
static bool shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
static void onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer);

/* -- Implementation of public functions -- */
void APP_Canard_initInstance()
{
    canardInit(
        &g_canardInstance,
        g_canardMemoryPool,
        APP_CANARD_MEMORY_POOL_SIZE,
        onTransferReceived,
        shouldAcceptTransfer,
        NULL
    );

    canardSetLocalNodeID(&g_canardInstance, APP_UAVCAN_DEFAULT_NODE_ID);
}

CanardInstance* APP_Canard_getInstance()
{
    return &g_canardInstance;
}

/* -- Implementation of public AO functions -- */
void UavcanNode_ctor(UavcanNode* me)
{
    QActive_ctor(&me->super, (QStateHandler) &UavcanNode_initial);
    QTimeEvt_ctorX(&(me->spinTimeEvent), &(me->super), SPIN_TIMEOUT_SIG, 0U);
}

/* -- Implementation of internal AO functions -- */
QState UavcanNode_initial(UavcanNode* me, QEvent const* e)
{
    QTimeEvt_armX(&me->spinTimeEvent, BSP_TICKS_PER_SEC/2, BSP_TICKS_PER_SEC/2);

    return Q_TRAN(&UavcanNode_idle);
}

QState UavcanNode_spinning(UavcanNode* me, QEvent const* e)
{
    QState status_;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            spin();
            BSP_CAN_sendAll(APP_Canard_getInstance());
            BSP_CAN_receiveOnce(APP_Canard_getInstance());
            status_ = Q_HANDLED();
            break;
        case SPIN_TIMEOUT_SIG:
            status_ = Q_TRAN(&UavcanNode_idle);
            break;
        default:
            status_ = Q_SUPER(&QHsm_top);
            break;
    }
    return status_;
}

QState UavcanNode_idle(UavcanNode* me, QEvent const* e)
{
    QState status_;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            status_ = Q_HANDLED();
            break;
        case SPIN_TIMEOUT_SIG:
            status_ = Q_TRAN(&UavcanNode_spinning);
            break;
        default:
            status_ = Q_SUPER(&QHsm_top);
            break;
    }
    return status_;
}

/* -- Implementation of Libcanard's callbacks -- */
static bool shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
    if ((transferType == CanardTransferTypeRequest) && (dataTypeId == UAVCAN_PROTOCOL_GETNODEINFO_ID)) {
        *outDataTypeSignature = UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE;
        return true;
    }
    return false;
}

static void onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer)
{
    if ((transfer->transfer_type == CanardTransferTypeRequest) && (transfer->data_type_id == UAVCAN_PROTOCOL_GETNODEINFO_ID)) {
        getNodeInfoHandle(transfer);
    }
}

/* -- Implementation of internal helper functions -- */
static void spin(void)
{
    static uint8_t transferId = 0; /* The transferId variable MUST BE STATIC; refer to the libcanard documentation for the explanation. */
    static uint8_t messageBuffer[UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE];

    const uint32_t len = makeNodeStatusMessage(messageBuffer);

    canardBroadcast(APP_Canard_getInstance(),
                    UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE,
                    UAVCAN_PROTOCOL_NODESTATUS_ID,
                    &transferId,
                    CANARD_TRANSFER_PRIORITY_LOW,
                    messageBuffer,
                    (uint16_t)len);
}

static uint32_t makeNodeStatusMessage(uint8_t* buffer)
{
    uavcan_protocol_NodeStatus nodeStatus;
    nodeStatus.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    nodeStatus.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    nodeStatus.uptime_sec = BSP_upTimeSeconds();

    return uavcan_protocol_NodeStatus_encode(&nodeStatus, buffer);
}

static uint32_t makeNodeInfoMessage(uint8_t* buffer)
{
    uavcan_protocol_GetNodeInfoResponse nodeInfoResponse;

    nodeInfoResponse.status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK;
    nodeInfoResponse.status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL;
    nodeInfoResponse.status.uptime_sec = BSP_upTimeSeconds();

    nodeInfoResponse.software_version.major = APP_SW_VERSION_MAJOR;
    nodeInfoResponse.software_version.minor = APP_SW_VERSION_MINOR;
    nodeInfoResponse.software_version.optional_field_flags = 1;
    nodeInfoResponse.software_version.vcs_commit = APP_SW_GIT_COMMIT_HASH; /* DEFINED BY CMAKE */

    nodeInfoResponse.hardware_version.major = APP_HW_VERSION_MAJOR;
    nodeInfoResponse.hardware_version.minor = APP_HW_VERSION_MINOR;

    BSP_readUniqueID(&nodeInfoResponse.hardware_version.unique_id[0]);    /* Writes unique ID into the buffer */
    
    return uavcan_protocol_GetNodeInfoResponse_encode(&nodeInfoResponse, buffer);
}

static void getNodeInfoHandle(CanardRxTransfer* transfer)
{
    uint8_t buffer[UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH];
    const uint32_t len = makeNodeInfoMessage(buffer);
    int result = canardRequestOrRespond(&g_canardInstance,
                                        transfer->source_node_id,
                                        UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
                                        UAVCAN_PROTOCOL_GETNODEINFO_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        &buffer[0],
                                        (uint16_t)len);
    if (result < 0)
    {
        /* TODO: handle the error */
    }
}