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

void UavcanNode_ctor(UavcanNode* me)
{
    QActive_ctor(&me->super, Q_STATE_CAST(&UavcanNode_initial));

    /* Publish status message every 350-700 ms (should be between 2 and 1000 ms). */
    me->publishStatusTimeout = 14U + (BSP_getPseudoRandom() % 16U);

    QTimeEvt_ctorX(&me->timeEvent, &me->super, UAVCAN_TIMEOUT_SIG, 0U);
}

/* -- Implementation of internal AO functions -- */
static QState UavcanNode_initial(UavcanNode* me, QEvt const * const e)
{
    (void) e; /* unused parameter */

    if(!BSP_CAN_init(BSP_CAN_getFilterConfig())) {
      return Q_TRAN(&UavcanNode_offline);
    }

    /* Initialize our Libcanard's instance */
    initCanardInstance();

    /* We're online at this point */
    return Q_TRAN(&UavcanNode_online);
}

static QState UavcanNode_online(UavcanNode* me, QEvt const * const e)
{
    QState status;
    switch(e->sig) {
        case Q_INIT_SIG:
            QTimeEvt_armX(&me->timeEvent, BSP_TICKS_PER_SEC / 1000U * 25U, BSP_TICKS_PER_SEC / 1000U * 25U); /* every 25 ms */
            me->publishStatusTimeoutCounter = 0U;
            status = Q_TRAN(&UavcanNode_spin);
            break;
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_RESTART_SIG:
            status = Q_TRAN(&UavcanNode_aboutToRestart);
            break;
        default:
            status = Q_SUPER(&QHsm_top);
            break;
    }
    return status;
}

static QState UavcanNode_offline(UavcanNode* me, QEvt const * const e)
{
  QState status;

  switch(e->sig) {
      case Q_INIT_SIG:
          QTimeEvt_armX(&me->timeEvent, BSP_TICKS_PER_SEC / 2U, BSP_TICKS_PER_SEC / 2U); /* every 500 ms */
          status = Q_HANDLED();
          break;
      case Q_ENTRY_SIG:
          status = Q_HANDLED();
          break;
      case Q_EXIT_SIG:
          status = Q_HANDLED();
          break;
      case UAVCAN_TIMEOUT_SIG:
          if(!BSP_CAN_init(BSP_CAN_getFilterConfig())) {
              status = Q_HANDLED();
          } else {
              /* Initialize our Libcanard's instance */
              initCanardInstance();
              /* We're online at this point */
              QTimeEvt_disarm(&me->timeEvent);
              status = Q_TRAN(&UavcanNode_online);
          }
          break;
      default:
          status = Q_SUPER(&QHsm_top);
          break;
  }
  return status;
}


static QState UavcanNode_spin(UavcanNode* me, QEvt const * const e)
{
    QState status;

    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            sendAll();
            status = Q_HANDLED();
            break;
        case UAVCAN_TIMEOUT_SIG:
          if(me->publishStatusTimeoutCounter == me->publishStatusTimeout) {
            me->publishStatusTimeoutCounter = 0U;
            statusUpdate();
          } else {
            me->publishStatusTimeoutCounter++;
          }
          sendAll();
          receiveAll();
          status = Q_HANDLED();
          break;
        default:
           status = Q_SUPER(&UavcanNode_online);
           break;
    }
    return status;
}

static QState UavcanNode_aboutToRestart(UavcanNode* me, QEvt const * const e)
{
    (void) me; /* not used yet */

    QState status;

    switch(e->sig) {
        case Q_ENTRY_SIG:
            status = Q_HANDLED();
            break;
        case Q_EXIT_SIG:
            status = Q_HANDLED();
            break;
        case UAVCAN_TIMEOUT_SIG:
            BSP_restart();
            status = Q_HANDLED();
            break;
        default:
           status = Q_HANDLED(); /* ignore all the other signals */
           break;
    }
    return status;
}
