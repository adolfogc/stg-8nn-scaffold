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

#ifndef _UAVCAN_NODE_AO_H
#define _UAVCAN_NODE_AO_H

#include <stdint.h>
#include "bsp_qpc.h"

typedef struct {
    QActive super;

    QTimeEvt timeEvent;
    /* Timer event used for publishing the status message. */
    uint32_t publishStatusTimeout;
    uint32_t publishStatusTimeoutCounter;
} UavcanNode;

enum UavcanNodeSignals {
    UAVCAN_TIMEOUT_SIG = Q_USER_SIG,
    UAVCAN_SPIN_SIG,
    UAVCAN_RESTART_SIG
};

void UavcanNode_ctor(UavcanNode* me);

static QState UavcanNode_initial(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_online(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_offline(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_spin(UavcanNode* me, QEvt const * const e);
static QState UavcanNode_aboutToRestart(UavcanNode* me, QEvt const * const e);

#endif /* _UAVCAN_NODE_AO_H */
