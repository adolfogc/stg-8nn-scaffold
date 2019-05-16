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

static bool shouldAcceptTransferDefault(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId)
{
    (void)instance;
    (void)sourceNodeId; /* not used yet */

    bool accept = false;
    switch(dataTypeId) {
        case UAVCAN_PROTOCOL_GETNODEINFO_ID:
            *outDataTypeSignature = UAVCAN_PROTOCOL_GETNODEINFO_SIGNATURE;
            accept = true;
            break;
        case UAVCAN_PROTOCOL_RESTARTNODE_ID:
            *outDataTypeSignature = UAVCAN_PROTOCOL_RESTARTNODE_SIGNATURE;
            accept = true;
            break;
        default:
            accept = shouldAcceptTransferExtend(instance, outDataTypeSignature, dataTypeId, transferType, sourceNodeId);
            break;
    }

    return accept;
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

    switch(transfer->data_type_id) {
        case UAVCAN_PROTOCOL_GETNODEINFO_ID:
          if(transfer->transfer_type == CanardTransferTypeRequest) {
            getNodeInfoHandle(transfer);
          }
          break;
        case UAVCAN_PROTOCOL_RESTARTNODE_ID:
          if(transfer->transfer_type == CanardTransferTypeRequest) {
            restartNodeHandle(transfer);
          }
          break;
        default:
          onTransferReceivedExtend(instance, transfer);
          break;
    }
}

static void onTransferReceivedExtendDefault(CanardInstance* instance, CanardRxTransfer* transfer)
{
  (void)instance;
  (void)transfer;
}
