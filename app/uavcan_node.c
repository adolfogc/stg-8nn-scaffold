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
static UavcanNode l_uavcanNode;

/* -- Active Object signals -- */
enum UavcanNodeSignals {
    UAVCAN_TIMEOUT_SIG = Q_USER_SIG,
    UAVCAN_SPIN_SIG,
    UAVCAN_RESTART_SIG
};

/* -- Libcanard's memory pool -- */
#define APP_CANARD_MEMORY_POOL_SIZE 4096

static uint8_t l_canardMemoryPool[APP_CANARD_MEMORY_POOL_SIZE];

/* -- Libcanard's instance -- */
static CanardInstance l_canardInstance;

static QState UavcanNode_init(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_online(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_offline(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_spin(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_aboutToRestart(UavcanNode* me, QEvt const * const e);

/* -- Prototypes for internal helper functions -- */
void UavcanNode_ctor(UavcanNode* me);

static void initCanardInstance(void);
static void statusUpdate(void);
static void sendAll(void);
static void receiveAll(void);

static uint32_t makeNodeStatusMessage(uint8_t* buffer);
static uint32_t makeNodeInfoMessage(uint8_t* buffer);
static void getNodeInfoHandle(CanardRxTransfer* transfer);
static void restartNodeHandle(CanardRxTransfer* transfer);

/* -- Prototypes for Libcanard's callback -- */
static bool shouldAcceptTransferDefault(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
bool shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId) __attribute__((weak, alias("shouldAcceptTransferDefault")));

static void onTransferReceivedDefault(CanardInstance* instance, CanardRxTransfer* transfer);
void onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer) __attribute__((weak, alias("onTransferReceivedDefault")));

static bool shouldAcceptTransferExtendDefault(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
bool shouldAcceptTransferExtend(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId) __attribute__((weak, alias("shouldAcceptTransferExtendDefault")));

static void onTransferReceivedExtendDefault(CanardInstance* instance, CanardRxTransfer* transfer);
void onTransferReceivedExtend(CanardInstance* instance, CanardRxTransfer* transfer) __attribute__((weak, alias("onTransferReceivedExtendDefault")));

/* -- Implementation of public AO functions -- */
UavcanNode* UavcanNode_initAO(void)
{
    UavcanNode_ctor(&l_uavcanNode);
    return &l_uavcanNode;
}

CanardInstance* getCanardInstance(void)
{
    return &l_canardInstance;
}


void UavcanNode_ctor(UavcanNode* me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&UavcanNode_init));
    QTimeEvt_ctorX(&me->timeEvent, &me->super, UAVCAN_TIMEOUT_SIG, 0U);
}

/* -- Implementation of internal AO functions -- */
static QState UavcanNode_init(UavcanNode* me, QEvt const * const e)
{
    (void) e; /* unused parameter */

    if(!BSP_CAN_init()) {
      return Q_TRAN(&UavcanNode_offline);
    }

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
            QTimeEvt_armX(&me->timeEvent, BSP_TICKS_PER_SEC / 1000U * 25U, BSP_TICKS_PER_SEC / 1000U * 25U); /* every 25 ms */
            me->spinCtrl = 0U;
            status = Q_TRAN(&UavcanNode_spin);
            break;
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_RESTART_SIG:
            status = Q_TRAN(&UavcanNode_aboutToRestart);
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
      case Q_INIT_SIG:
          QTimeEvt_armX(&me->timeEvent, BSP_TICKS_PER_SEC / 2U, BSP_TICKS_PER_SEC / 2U); /* every 500 ms */
          status = Q_HANDLED();
          break;
      case Q_ENTRY_SIG:
          status = Q_HANDLED();
          break;
      case Q_EXIT_SIG:
          status = Q_HANDLED();
          break;
      case UAVCAN_TIMEOUT_SIG:
          if(!BSP_CAN_init()) {
              status = Q_HANDLED();
          } else {
              /* Initialize our Libcanard's instance */
              initCanardInstance();
              /* We're online at this point */
              QTimeEvt_disarm(&me->timeEvent);
              status = Q_TRAN(&UavcanNode_online);
          }
          break;
      default:
          status = Q_SUPER(&QHsm_top);
          break;
  }
  return status;
}


static QState UavcanNode_spin(UavcanNode* me, QEvt const * const e)
{
    QState status;

    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            sendAll();
            status = Q_HANDLED();
            break;
        case UAVCAN_TIMEOUT_SIG:
          if(me->spinCtrl == 8U) { /* every 200 ms */
            me->spinCtrl = 0U;
            statusUpdate();
          } else {
            me->spinCtrl++;
          }
          sendAll();
          receiveAll();
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
    (void) me; /* not used yet */

    QState status;

    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_TIMEOUT_SIG:
            BSP_restart();
            status = Q_HANDLED();
            break;
        default:
           status = Q_HANDLED(); /* ignore all the other signals */
           break;
    }
    return status;
}

/* -- Implementation of Libcanard's callbacks -- */
static bool shouldAcceptTransferDefault(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
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
            return shouldAcceptTransferExtend(instance, outDataTypeSignature, dataTypeId, transferType, sourceNodeId);
    }
}

static bool shouldAcceptTransferExtendDefault(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
    (void)instance;
    (void)outDataTypeSignature;
    (void)dataTypeId;
    (void)transferType;
    (void)sourceNodeId; /* not used yet */

    return false;
}


static void onTransferReceivedDefault(CanardInstance* instance, CanardRxTransfer* transfer)
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
            onTransferReceivedExtend(instance, transfer);
    }
}

static void onTransferReceivedExtendDefault(CanardInstance* instance, CanardRxTransfer* transfer)
{
  (void)instance;
  (void)transfer;
  return;
}

/* -- Implementation of internal helper functions -- */
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
