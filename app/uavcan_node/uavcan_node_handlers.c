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

static void statusUpdate(void) {
  /* The transferId variable MUST BE STATIC; refer to the libcanard documentation for the explanation. */
  static uint8_t transferId = 0U;

  uint8_t messageBuffer[UAVCAN_PROTOCOL_NODESTATUS_MAX_SIZE] = {0U};
  const uint32_t len = makeNodeStatusMessage(messageBuffer);

  canardBroadcast(&l_canardInstance, UAVCAN_PROTOCOL_NODESTATUS_SIGNATURE, UAVCAN_PROTOCOL_NODESTATUS_ID, &transferId,
                  CANARD_TRANSFER_PRIORITY_LOW, messageBuffer, (uint16_t)len);
}

static void restartNodeHandle(CanardRxTransfer *transfer) {
  uint8_t buffer[UAVCAN_PROTOCOL_RESTARTNODE_REQUEST_MAX_SIZE] = {0U};

  uavcan_protocol_RestartNodeRequest request = {0};
  uavcan_protocol_RestartNodeRequest_decode(transfer, transfer->payload_len, &request, (uint8_t **)0U);

  uavcan_protocol_RestartNodeResponse response = {.ok = request.magic_number ==
                                                        UAVCAN_PROTOCOL_RESTARTNODE_REQUEST_MAGIC_NUMBER};

  const uint32_t len = uavcan_protocol_RestartNodeResponse_encode(&response, buffer);

  int result =
      canardRequestOrRespond(&l_canardInstance, transfer->source_node_id, UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE,
                             UAVCAN_PROTOCOL_RESTARTNODE_ID, &transfer->transfer_id, transfer->priority, CanardResponse,
                             buffer, (uint16_t)len);

  if (result >= 0 && response.ok) {
    static const QEvt aboutToRestartEvt = {RESTART_SIG, 0U, 0U};
    QACTIVE_POST(&AO_uavcanNode->super, &aboutToRestartEvt, (void *)0U);
  }
}

static void getNodeInfoHandle(CanardRxTransfer *transfer) {
  uint8_t messageBuffer[UAVCAN_PROTOCOL_GETNODEINFO_RESPONSE_NAME_MAX_LENGTH] = {0U};
  const uint32_t len = makeNodeInfoMessage(messageBuffer);

  int result =
      canardRequestOrRespond(&l_canardInstance, transfer->source_node_id, UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE,
                             UAVCAN_PROTOCOL_GETNODEINFO_ID, &transfer->transfer_id, transfer->priority, CanardResponse,
                             messageBuffer, (uint16_t)len);
  (void)result; /* Unused */
}

static uint32_t makeNodeStatusMessage(uint8_t *messageBuffer) {
  uavcan_protocol_NodeStatus nodeStatus = {.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK,
                                           .mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL,
                                           .uptime_sec = BSP_upTimeSeconds()};

  return uavcan_protocol_NodeStatus_encode(&nodeStatus, messageBuffer);
}

static uint32_t makeNodeInfoMessage(uint8_t *messageBuffer) {
  uavcan_protocol_GetNodeInfoResponse nodeInfoResponse = {.name.data = (uint8_t *)APP_UAVCAN_NODE_NAME_DATA,
                                                          .name.len = APP_UAVCAN_NODE_NAME_LEN,

                                                          .status.health = UAVCAN_PROTOCOL_NODESTATUS_HEALTH_OK,
                                                          .status.mode = UAVCAN_PROTOCOL_NODESTATUS_MODE_OPERATIONAL,
                                                          .status.uptime_sec = BSP_upTimeSeconds(),

                                                          .software_version.major = APP_SW_VERSION_MAJOR,
                                                          .software_version.minor = APP_SW_VERSION_MINOR,
                                                          .software_version.optional_field_flags = 1U,
                                                          .software_version.vcs_commit =
                                                              APP_SW_GIT_COMMIT_HASH, /* DEFINED BY CMAKE */

                                                          .hardware_version.major = APP_HW_VERSION_MAJOR,
                                                          .hardware_version.minor = APP_HW_VERSION_MINOR};

  BSP_readUniqueID(&(nodeInfoResponse.hardware_version.unique_id[0])); /* Writes unique ID into the buffer */

  return uavcan_protocol_GetNodeInfoResponse_encode(&nodeInfoResponse, messageBuffer);
}
