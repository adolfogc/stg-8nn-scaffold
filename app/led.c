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
#include "led.h"

enum LedSignals {
    TIMEOUT_SIG = Q_USER_SIG,
    MAX_SIG
};

/* Prototypes */
static QState Led_initial(Led * const me, QEvent const * const e);
static QState Led_off(Led * const me, QEvent const * const e);
static QState Led_on(Led * const me, QEvent const * const e);

/* Implementations */
void Led_ctor(Led* me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&Led_initial));
    QTimeEvt_ctorX(&(me->timeEvent), &(me->super), TIMEOUT_SIG, 0U);
}

static QState Led_initial(Led * const me, QEvent const * const e)
{
    /* Arm the private time event to expire in 1s and periodically every 1 seconds */
    QTimeEvt_armX(&me->timeEvent,
        1*BSP_TICKS_PER_SEC,
        1*BSP_TICKS_PER_SEC);
    return Q_TRAN(&Led_off);
}

static QState Led_off(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:
          BSP_Led_off();
          status_ = Q_HANDLED();
          break;
        case TIMEOUT_SIG:
          status_ = Q_TRAN(&Led_on);
          break;
        default:
          status_ = Q_SUPER(&QHsm_top);
          break;
    }
    return status_;
}

static QState Led_on(Led * const me, QEvent const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:
          BSP_Led_on();
          status_ = Q_HANDLED();
          break;
        case TIMEOUT_SIG:
          status_ = Q_TRAN(&Led_off);
          break;
        default:
          status_ = Q_SUPER(&QHsm_top);
          break;
    }
    return status_;
}
