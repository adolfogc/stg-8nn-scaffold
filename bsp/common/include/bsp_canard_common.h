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

#ifndef _BSP_CANARD_COMMON_H
#define _BSP_CANARD_COMMON_H

#include <stdint.h>
#include "canard.h"

/* -- Canard callbacks -- */
bool BSP_Canard_shouldAcceptTransfer(const CanardInstance* instance, uint64_t* outDataTypeSignature, uint16_t dataTypeId, CanardTransferType transferType, uint8_t sourceNodeId);
void BSP_Canard_onTransferReceived(CanardInstance* instance, CanardRxTransfer* transfer);

/* -- Helper functions -- */
void BSP_Canard_makeNodeStatusMessage(uint8_t* buffer);
uint16_t BSP_Canard_makeNodeInfoMessage(uint8_t* buffer);
void BSP_Canard_getNodeInfoHandle(CanardRxTransfer* transfer);

/* -- Canard library instance -- */
extern CanardInstance g_canard;

#endif /* _BSP_CANARD_COMMON_H */