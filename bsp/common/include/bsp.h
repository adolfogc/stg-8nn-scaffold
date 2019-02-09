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

#ifndef _BSP_H
#define _BSP_H

#include <stdint.h>
#include "canard.h"
#include "bsp_specific.h"

void BSP_init(void);
void BSP_restart(void);

void BSP_Led_off(void);
void BSP_Led_on(void);

void BSP_CAN_init(void);

enum BSP_CAN_RxTxResultTag {
    BSP_CAN_RXTX_TIMEOUT,
    BSP_CAN_RXTX_SUCCESS,
    BSP_CAN_RXTX_ERROR
};

typedef enum BSP_CAN_RxTxResultTag BSP_CAN_RxTxResult;

BSP_CAN_RxTxResult BSP_CAN_transmitOnce(const CanardCANFrame* frame);
BSP_CAN_RxTxResult BSP_CAN_receiveOnce(CanardCANFrame* frame);

uint32_t BSP_upTimeSeconds(void);
void BSP_readUniqueID(uint8_t* outUid);

#endif /* _BSP_H */