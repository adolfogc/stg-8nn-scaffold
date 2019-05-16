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

#include "uavcan_node.h"
#include "app.h"
#include "canard.h"
#include "uavcan/protocol/GetNodeInfo.h"
#include "uavcan/protocol/NodeStatus.h"
#include "uavcan/protocol/RestartNode.h"
#include "uavcan_node_ao.h"
#include "uavcan_node_callbacks.h"
#include "uavcan_node_handlers.h"
#include "uavcan_node_helpers.h"
#include <stdbool.h>
#include <string.h>

Q_DEFINE_THIS_FILE

/* -- Active object instance -- */
static UavcanNode l_uavcanNode;
static QEvt const *l_uavcanNode_evtBuffer[APP_UAVCAN_NODE_EVT_QUEUE_SIZE];

/* -- Libcanard's memory pool -- */
static uint8_t l_canardMemoryPool[APP_CANARD_MEMORY_POOL_SIZE];

/* -- Libcanard's instance -- */
static CanardInstance l_canardInstance;

/* -- Active object implementation -- */
#include "uavcan_node_ao.c"

/* -- UAVCAN Callbacks -- */
#include "uavcan_node_callbacks.c"

/* -- UAVCAN handlers -- */
#include "uavcan_node_handlers.c"

/* -- Helper functions -- */
#include "uavcan_node_helpers.c"

/* -- Implementation of public AO functions -- */
void UavcanNode_initAO(void) { UavcanNode_ctor(&l_uavcanNode); }

void UavcanNode_startAO(uint8_t priority) {
  QACTIVE_START((QActive *)&l_uavcanNode.super, priority, l_uavcanNode_evtBuffer, Q_DIM(l_uavcanNode_evtBuffer),
                (void *)0, 0U, (QEvt *)0);
}

CanardInstance *getCanardInstance(void) { return &l_canardInstance; }
