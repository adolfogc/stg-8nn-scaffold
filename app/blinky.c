/* 
Copyright (C) 2018 Adolfo E. García

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

#include "qpc.h"
#include "bsp.h"
#include "blinky.h"

/* TEMP */
#define BSP_TICKS_PER_SEC 1000
void BSP_ledOff(void);
void BSP_ledOn(void);

void BSP_ledOff(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);
}

void BSP_ledOn(void)
{
    HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}

/* TEMP*/

/* Blinky class */
typedef struct {
/* protected: */
    QActive super;
/* private: */
    QTimeEvt timeEvt;
} Blinky;

/* protected: */
static QState Blinky_initial(Blinky * const me, QEvt const * const e);
static QState Blinky_off(Blinky * const me, QEvt const * const e);
static QState Blinky_on(Blinky * const me, QEvt const * const e);

/* instantiate the Blinky active object ------------------------------------*/
static Blinky l_blinky;
QActive * const AO_Blinky = &l_blinky.super;

void Blinky_ctor(void) {
    Blinky *me = (Blinky *)AO_Blinky;
    QActive_ctor(&me->super, Q_STATE_CAST(&Blinky_initial));
    QTimeEvt_ctorX(&me->timeEvt, &me->super, TIMEOUT_SIG, 0U);
}

static QState Blinky_initial(Blinky * const me, QEvt const * const e) {
    /* arm the private time event to expire in 1s
     * and periodically every 1 seconds
     */
    QTimeEvt_armX(&me->timeEvt,
        1*BSP_TICKS_PER_SEC,
        1*BSP_TICKS_PER_SEC);
    return Q_TRAN(&Blinky_off);
}
static QState Blinky_off(Blinky * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:
          BSP_ledOff();
          status_ = Q_HANDLED();
          break;
        case TIMEOUT_SIG:
          status_ = Q_TRAN(&Blinky_on);
          break;
        default:
          status_ = Q_SUPER(&QHsm_top);
          break;
    }
    return status_;
}

static QState Blinky_on(Blinky * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        case Q_ENTRY_SIG:
          BSP_ledOn();
          status_ = Q_HANDLED();
          break;
        case TIMEOUT_SIG:
          status_ = Q_TRAN(&Blinky_off);
          break;
        default:
          status_ = Q_SUPER(&QHsm_top);
          break;
    }
    return status_;
}
