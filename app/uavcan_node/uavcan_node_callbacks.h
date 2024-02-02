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

#ifndef _UAVCAN_NODE_CALLBACKS_H
#define _UAVCAN_NODE_CALLBACKS_H

#include <stdbool.h>

#include "canard.h"

/* -- Prototypes for Libcanard's callback -- */
bool shouldAcceptTransferDefault(const CanardInstance *instance, uint64_t *outDataTypeSignature,
                                        uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);

bool shouldAcceptTransfer(const CanardInstance *instance, uint64_t *outDataTypeSignature, uint16_t dataTypeId,
                          CanardTransferType transferType, uint8_t sourceNodeId)
    __attribute__((weak, alias("shouldAcceptTransferDefault")));

void onTransferReceivedDefault(CanardInstance *instance, CanardRxTransfer *transfer);

void onTransferReceived(CanardInstance *instance, CanardRxTransfer *transfer)
    __attribute__((weak, alias("onTransferReceivedDefault")));

bool shouldAcceptTransferExtendDefault(const CanardInstance *instance, uint64_t *outDataTypeSignature,
                                              uint16_t dataTypeId, CanardTransferType transferType,
                                              uint8_t sourceNodeId);

bool shouldAcceptTransferExtend(const CanardInstance *instance, uint64_t *outDataTypeSignature, uint16_t dataTypeId,
                                CanardTransferType transferType, uint8_t sourceNodeId)
    __attribute__((weak, alias("shouldAcceptTransferExtendDefault")));

void onTransferReceivedExtendDefault(CanardInstance *instance, CanardRxTransfer *transfer);

void onTransferReceivedExtend(CanardInstance *instance, CanardRxTransfer *transfer)
    __attribute__((weak, alias("onTransferReceivedExtendDefault")));

#endif /* _UAVCAN_NODE_CALLBACKS_H */
