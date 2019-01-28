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

#include "bsp.h"
#include "uavcan_node.h"

static UavcanNode l_uavcanNode;
QActive * const AO_UavcanNode = &l_uavcanNode.super;

enum UavcanNodeSignals {
    SPIN_TIMEOUT_SIG = Q_USER_SIG,
    MAX_SIG
};

/* -- Prototypes of internal functions -- */
QState UavcanNode_initial(UavcanNode* me, QEvent const* e);
QState UavcanNode_spinning(UavcanNode* me, QEvent const* e);
QState UavcanNode_idle(UavcanNode* me, QEvent const* e);

/* -- Implementation of public functions -- */
void UavcanNode_ctor(UavcanNode* me)
{
    QActive_ctor(&me->super, (QStateHandler) &UavcanNode_initial);
    QTimeEvt_ctorX(&(me->spinTimeEvent), &(me->super), SPIN_TIMEOUT_SIG, 0U);
}

/* -- Implementation of internal functions -- */
QState UavcanNode_initial(UavcanNode* me, QEvent const* e)
{
    me->pollCtr = 0;
    me->procCtr = 0;

    QTimeEvt_armX(&me->spinTimeEvent, BSP_TICKS_PER_SEC/2, BSP_TICKS_PER_SEC/2);

    return Q_TRAN(&UavcanNode_idle);
}

QState UavcanNode_spinning(UavcanNode* me, QEvent const* e)
{
    QState status_;
    switch(e->sig) {
        case Q_ENTRY_SIG:
            BSP_Canard_spin();
            BSP_Canard_send();
            BSP_Canard_receive();
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