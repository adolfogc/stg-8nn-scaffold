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

#include <string.h>
#include <stdbool.h>
#include "canard.h"
#include "uavcan/protocol/GetNodeInfo.h"
#include "uavcan/protocol/NodeStatus.h"
#include "uavcan/protocol/RestartNode.h"
#include "bsp.h"
#include "app.h"
#include "uavcan_node.h"

/* -- Active Object instance -- */
UavcanNode g_uavcanNode;

/* -- Active Object signals -- */
enum UavcanNodeSignals {
    UAVCAN_STATUSUPDATE_SIG = Q_USER_SIG,
    UAVCAN_RECEIVE_SIG,
    UAVCAN_ABOUTTORESTART_SIG,
    UAVCAN_RESTART_SIG,
    UAVCAN_CONFIG_SIG,
    UAVCAN_OFFLINE_SIG
};

/* -- Libcanard's memory pool -- */
#define APP_CANARD_MEMORY_POOL_SIZE 2048

static uint8_t g_canardMemoryPool[APP_CANARD_MEMORY_POOL_SIZE];

/* -- Libcanard's instance -- */
CanardInstance g_canardInstance;

static QState UavcanNode_init(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_online(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_offline(UavcanNode* me, QEvt const * const e);

static QState UavcanNode_idle(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_aboutToRestart(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_statusUpdate(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_receive(UavcanNode* me, QEvt const * const e);

/* -- Prototypes for internal helper functions -- */
void UavcanNode_ctor(UavcanNode* me);

static void initCanardInstance(void);
static void statusUpdate(void);
static bool sendOnce(void);
static void sendAll(void);
static void receiveOnce(void);

static uint32_t makeNodeStatusMessage(uint8_t* buffer);
static uint32_t makeNodeInfoMessage(uint8_t* buffer);
static void getNodeInfoHandle(CanardRxTransfer* transfer);
static void restartNodeHandle(CanardRxTransfer* transfer);

/* -- Prototypes for Libcanard's callback -- */
static bool shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
static void onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer);

/* -- Implementation of public AO functions -- */
UavcanNode* initUavcanNode(void)
{
    UavcanNode_ctor(&g_uavcanNode);
    return &g_uavcanNode;
}

void UavcanNode_ctor(UavcanNode* me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&UavcanNode_init));
    QTimeEvt_ctorX(&me->statusUpdateTimeEvent, &me->super, UAVCAN_STATUSUPDATE_SIG, 0U);
    QTimeEvt_ctorX(&me->receiveTimeEvent, &me->super, UAVCAN_RECEIVE_SIG, 0U);
    QTimeEvt_ctorX(&me->restartTimeEvent, &me->super, UAVCAN_RESTART_SIG, 0U); 
}

/* -- Implementation of internal AO functions -- */
static QState UavcanNode_init(UavcanNode* me, QEvt const * const e)
{
    (void) e; /* unused parameter */

    /* Setup the broadcast timer */
    QTimeEvt_armX(&me->statusUpdateTimeEvent, BSP_TICKS_PER_SEC / 2U, BSP_TICKS_PER_SEC / 2U); /* every 500 ms */
    QTimeEvt_armX(&me->receiveTimeEvent, BSP_TICKS_PER_SEC / 4U, BSP_TICKS_PER_SEC / 4U);      /* every 100 ms */

    /* Initialize our Libcanard's instance */
    initCanardInstance();

    /* We're online at this point */
    return Q_TRAN(&UavcanNode_online);
}

static QState UavcanNode_online(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_INIT_SIG:
            status = Q_TRAN(&UavcanNode_idle);
            break;            
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_STATUSUPDATE_SIG:
            status = Q_TRAN(&UavcanNode_statusUpdate);
            break;
        case UAVCAN_RECEIVE_SIG:
            status = Q_TRAN(&UavcanNode_receive);
            break;
        case UAVCAN_ABOUTTORESTART_SIG:
            status = Q_TRAN(&UavcanNode_aboutToRestart);
            break;
        case UAVCAN_OFFLINE_SIG:
            status = Q_TRAN(&UavcanNode_offline);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}


static QState UavcanNode_offline(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        default:
           status = Q_SUPER(&QHsm_top);
           break; 
    }
    return status;
}

static QState UavcanNode_idle(UavcanNode* me, QEvt const * const e)
{
    QState status;

    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        default:
           status = Q_SUPER(&UavcanNode_online);
           break; 
    }
    return status;
}

static QState UavcanNode_aboutToRestart(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            QTimeEvt_armX(&me->restartTimeEvent, BSP_TICKS_PER_SEC, 0U);
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_RESTART_SIG:
            BSP_restart();
            status = Q_HANDLED();
            break;
        default:
           status = Q_HANDLED(); /* ignore all the other signals */
           break; 
    }
    return status;
}

static QState UavcanNode_statusUpdate(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            statusUpdate();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        default:
           status = Q_SUPER(&UavcanNode_online);
           break; 
    }
    return status;
}

static QState UavcanNode_receive(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            receiveOnce();
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        default:
           status = Q_SUPER(&UavcanNode_online);
           break; 
    }
    return status;
}

/* -- Implementation of Libcanard's callbacks -- */
static bool shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
    (void)instance;
    (void)sourceNodeId; /* not used yet */

    if (transferType != CanardTransferTypeRequest) {
        return false;
    }

    switch(dataTypeId) {
        case UAVCAN_PROTOCOL_GETNODEINFO_ID:
            *outDataTypeSignature = UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE;
            return true;
        case UAVCAN_PROTOCOL_RESTARTNODE_ID:
            *outDataTypeSignature = UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE;
            return true;
        default:
            return false;
    }
}

static void onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer)
{
    (void)instance; /* not used yet */

    if(transfer->transfer_type != CanardTransferTypeRequest) {
        return;
    }

    switch(transfer->data_type_id) {
        case UAVCAN_PROTOCOL_GETNODEINFO_ID:
            getNodeInfoHandle(transfer);
            return;
        case UAVCAN_PROTOCOL_RESTARTNODE_ID:
            restartNodeHandle(transfer);
            return;
        default:
            return;
    }
}

/* -- Implementation of internal helper functions -- */
static void initCanardInstance(void)
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

static void statusUpdate(void)
{
    static uint8_t transferId = 0; /* The transferId variable MUST BE STATIC; refer to the libcanard documentation for the explanation. */
    
    uint8_t messageBuffer[UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE];
    memset(messageBuffer, 0, UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE);

    const uint32_t len = makeNodeStatusMessage(messageBuffer);

    canardBroadcast(&g_canardInstance,
                    UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE,
                    UAVCAN_PROTOCOL_NODESTATUS_ID,
                    &transferId,
                    CANARD_TRANSFER_PRIORITY_LOW,
                    messageBuffer,
                    (uint16_t)len);
    sendAll();
}

static void sendAll(void)
{
    for(bool nextExists = true; nextExists; nextExists = sendOnce());
}

/* Returns true if another frame can be send */
static bool sendOnce(void)
{
    const CanardCANFrame* txFrame = canardPeekTxQueue(&g_canardInstance);
    bool nextExists = false;

    if(txFrame != NULL) {
        const BSP_CAN_RxTxResult result = BSP_CAN_transmitOnce(txFrame);
        switch (result) {
            case BSP_CAN_RXTX_TIMEOUT:
                /* TODO: handle case */
            break;
            case BSP_CAN_RXTX_SUCCESS:
                canardPopTxQueue(&g_canardInstance);
            break;
            case BSP_CAN_RXTX_ERROR:
                /* TODO: handle case */
            break;
        }
        nextExists = canardPeekTxQueue(&g_canardInstance) != NULL;
    }

    return nextExists;
}

static void receiveOnce(void)
{
    CanardCANFrame rxFrame;
    memset(&rxFrame, 0, sizeof(rxFrame));

    const BSP_CAN_RxTxResult result = BSP_CAN_receiveOnce(&rxFrame);

    switch (result) {
        case BSP_CAN_RXTX_TIMEOUT:
            /* TODO: handle case */
        break;
        case BSP_CAN_RXTX_SUCCESS:
            canardHandleRxFrame(&g_canardInstance, &rxFrame, BSP_upTimeSeconds());
        break;
        case BSP_CAN_RXTX_ERROR:
            /* TODO: handle case */
        break;
    }
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

static void getNodeInfoHandle(CanardRxTransfer* transfer)
{
    uint8_t messageBuffer[UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH];
    memset(messageBuffer, 0, UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH);

    const uint32_t len = makeNodeInfoMessage(messageBuffer);

    int result = canardRequestOrRespond(&g_canardInstance,
                                        transfer->source_node_id,
                                        UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
                                        UAVCAN_PROTOCOL_GETNODEINFO_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        messageBuffer,
                                        (uint16_t)len);
    if (result < 0) {
        /* TODO: handle the error */
    } else {
        sendAll();
    }
}

static void restartNodeHandle(CanardRxTransfer* transfer)
{
    uint8_t messageBuffer[UAVCAN_PROTOCOL_RESTARTNODE_RESPONSE_MAX_SIZE];

    uavcan_protocol_RestartNodeResponse restartNodeResponse;
    restartNodeResponse.ok = true;

    const uint32_t len = uavcan_protocol_RestartNodeResponse_encode(&restartNodeResponse, messageBuffer);

    int result = canardRequestOrRespond(&g_canardInstance,
                                        transfer->source_node_id,
                                        UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE,
                                        UAVCAN_PROTOCOL_RESTARTNODE_ID,
                                        &transfer->transfer_id,
                                        transfer->priority,
                                        CanardResponse,
                                        messageBuffer,
                                        (uint16_t)len);
    if (result < 0) {
        /* TODO: handle the error */
    } else {
        sendAll();
        static const QEvt aboutToRestartEvt = {UAVCAN_ABOUTTORESTART_SIG, 0U, 0U};
        QACTIVE_POST(&g_uavcanNode.super, &aboutToRestartEvt, (void*)0);
    }
}
