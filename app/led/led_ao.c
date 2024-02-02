//$file${.::led::led_ao.c} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: app.qm
// File:  ${.::led::led_ao.c}
//
// This code has been generated by QM 5.3.0 <www.state-machine.com/qm>.
// DO NOT EDIT THIS FILE MANUALLY. All your changes will be lost.
//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// This generated code is open source software: you can redistribute it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation.
//
// This code is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
// more details.
//
// NOTE:
// Alternatively, this generated code may be distributed under the terms
// of Quantum Leaps commercial licenses, which expressly supersede the GNU
// General Public License and are specifically designed for licensees
// interested in retaining the proprietary status of their code.
//
// Contact information:
// <www.state-machine.com/licensing>
// <info@state-machine.com>
//
//$endhead${.::led::led_ao.c} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#include "qpc.h"
#include "bsp_qpc.h"

#include "app.h"
#include "led_ao.h"
#include "led_ao_private.h"

/* Declarations */
//$declare${AOs::Led} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Led} ................................................................
typedef struct {
// protected:
    QMActive super;

// private:
    QTimeEvt timeEvt;
    uint32_t blinkPeriod;
} Led;

// protected:
static QState Led_initial(Led * const me, void const * const par);
static QState Led_blink  (Led * const me, QEvt const * const e);
static QState Led_blink_e(Led * const me);
static QState Led_blink_x(Led * const me);
static QState Led_blink_i(Led * const me);
static QMState const Led_blink_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Led_blink),
    Q_ACTION_CAST(&Led_blink_e),
    Q_ACTION_CAST(&Led_blink_x),
    Q_ACTION_CAST(&Led_blink_i)
};
static QState Led_blink_on  (Led * const me, QEvt const * const e);
static QState Led_blink_on_e(Led * const me);
static QMState const Led_blink_on_s = {
    &Led_blink_s, // superstate
    Q_STATE_CAST(&Led_blink_on),
    Q_ACTION_CAST(&Led_blink_on_e),
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState Led_blink_off  (Led * const me, QEvt const * const e);
static QState Led_blink_off_e(Led * const me);
static QMState const Led_blink_off_s = {
    &Led_blink_s, // superstate
    Q_STATE_CAST(&Led_blink_off),
    Q_ACTION_CAST(&Led_blink_off_e),
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState Led_off  (Led * const me, QEvt const * const e);
static QState Led_off_e(Led * const me);
static QMState const Led_off_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Led_off),
    Q_ACTION_CAST(&Led_off_e),
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
static QState Led_on  (Led * const me, QEvt const * const e);
static QState Led_on_e(Led * const me);
static QMState const Led_on_s = {
    QM_STATE_NULL, // superstate (top)
    Q_STATE_CAST(&Led_on),
    Q_ACTION_CAST(&Led_on_e),
    Q_ACTION_NULL, // no exit action
    Q_ACTION_NULL  // no initial tran.
};
//$enddecl${AOs::Led} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

/* Instance */
static Led l_led;

/* Instance pointers */
QActive * AO_led = &l_led.super.super;

/* Constructor */
void Led_ctor(QActive * ao) {
  Led* me = (Led*)ao;
  QMActive_ctor(&me->super, Q_STATE_CAST(&Led_initial));
  QTimeEvt_ctorX(&me->timeEvt, &me->super.super, TIMEOUT_SIG, 0U);
  me->blinkPeriod = LED_DEFAULT_BLINK_TIMEOUT;
}

/* Definitions */
//$skip${QP_VERSION} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
// Check for the minimum required QP version
#if (QP_VERSION < 730U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 7.3.0 or higher required
#endif
//$endskip${QP_VERSION} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

//$define${AOs::Led} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv

//${AOs::Led} ................................................................

//${AOs::Led::SM} ............................................................
static QState Led_initial(Led * const me, void const * const par) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { // tran-action table
        &Led_off_s, // target state
        {
            Q_ACTION_CAST(&Led_off_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    //${AOs::Led::SM::initial}
    return QM_TRAN_INIT(&tatbl_);
}

//${AOs::Led::SM::blink} .....................................................
//${AOs::Led::SM::blink}
static QState Led_blink_e(Led * const me) {
    QTimeEvt_armX(&me->timeEvt, me->blinkPeriod, me->blinkPeriod);
    return QM_ENTRY(&Led_blink_s);
}
//${AOs::Led::SM::blink}
static QState Led_blink_x(Led * const me) {
    QTimeEvt_disarm(&me->timeEvt);
    return QM_EXIT(&Led_blink_s);
}
//${AOs::Led::SM::blink::initial}
static QState Led_blink_i(Led * const me) {
    static struct {
        QMState const *target;
        QActionHandler act[2];
    } const tatbl_ = { // tran-action table
        &Led_blink_off_s, // target state
        {
            Q_ACTION_CAST(&Led_blink_off_e), // entry
            Q_ACTION_NULL // zero terminator
        }
    };
    //${AOs::Led::SM::blink::initial}
    return QM_TRAN_INIT(&tatbl_);
}
//${AOs::Led::SM::blink}
static QState Led_blink(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Led::SM::blink::LED_ON}
        case LED_ON_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &Led_on_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_x), // exit
                    Q_ACTION_CAST(&Led_on_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        //${AOs::Led::SM::blink::LED_BLINK}
        case LED_BLINK_SIG: {
            LedEvt* ledEvt = (LedEvt*)e;

            if(ledEvt->blinkPeriod > 0U) {
              me->blinkPeriod = ledEvt->blinkPeriod;
            }

            QTimeEvt_rearm(&me->timeEvt, me->blinkPeriod);
            status_ = QM_HANDLED();
            break;
        }
        //${AOs::Led::SM::blink::LED_OFF}
        case LED_OFF_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &Led_off_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_x), // exit
                    Q_ACTION_CAST(&Led_off_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${AOs::Led::SM::blink::blink_on} ...........................................
//${AOs::Led::SM::blink::blink_on}
static QState Led_blink_on_e(Led * const me) {
    BSP_Led_on();
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&Led_blink_on_s);
}
//${AOs::Led::SM::blink::blink_on}
static QState Led_blink_on(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Led::SM::blink::blink_on::TIMEOUT}
        case TIMEOUT_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { // tran-action table
                &Led_blink_off_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_off_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${AOs::Led::SM::blink::blink_off} ..........................................
//${AOs::Led::SM::blink::blink_off}
static QState Led_blink_off_e(Led * const me) {
    BSP_Led_off();
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&Led_blink_off_s);
}
//${AOs::Led::SM::blink::blink_off}
static QState Led_blink_off(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Led::SM::blink::blink_off::TIMEOUT}
        case TIMEOUT_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[2];
            } const tatbl_ = { // tran-action table
                &Led_blink_on_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_on_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${AOs::Led::SM::off} .......................................................
//${AOs::Led::SM::off}
static QState Led_off_e(Led * const me) {
    BSP_Led_off();
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&Led_off_s);
}
//${AOs::Led::SM::off}
static QState Led_off(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Led::SM::off::LED_BLINK}
        case LED_BLINK_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &Led_blink_off_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_e), // entry
                    Q_ACTION_CAST(&Led_blink_off_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            LedEvt* ledEvt = (LedEvt*)e;

            if(ledEvt->blinkPeriod > 0U) {
              me->blinkPeriod = ledEvt->blinkPeriod;
            }
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}

//${AOs::Led::SM::on} ........................................................
//${AOs::Led::SM::on}
static QState Led_on_e(Led * const me) {
    BSP_Led_on();
    Q_UNUSED_PAR(me);
    return QM_ENTRY(&Led_on_s);
}
//${AOs::Led::SM::on}
static QState Led_on(Led * const me, QEvt const * const e) {
    QState status_;
    switch (e->sig) {
        //${AOs::Led::SM::on::LED_BLINK}
        case LED_BLINK_SIG: {
            static struct {
                QMState const *target;
                QActionHandler act[3];
            } const tatbl_ = { // tran-action table
                &Led_blink_on_s, // target state
                {
                    Q_ACTION_CAST(&Led_blink_e), // entry
                    Q_ACTION_CAST(&Led_blink_on_e), // entry
                    Q_ACTION_NULL // zero terminator
                }
            };
            LedEvt* ledEvt = (LedEvt*)e;

            if(ledEvt->blinkPeriod > 0U) {
              me->blinkPeriod = ledEvt->blinkPeriod;
            }
            status_ = QM_TRAN(&tatbl_);
            break;
        }
        default: {
            status_ = QM_SUPER();
            break;
        }
    }
    return status_;
}
//$enddef${AOs::Led} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
