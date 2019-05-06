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

#ifndef _LED_AO_H
#define _LED_AO_H

#include "bsp_qpc.h"
#include "app_signals.h"

typedef struct {
    QActive super;

    QTimeEvt timeEvent;
} Led;

enum LedSignals {
    LED_ON_SIG = APP_LED_ON_SIG,
    LED_BLINK_SIG = APP_LED_BLINK_SIG,
    LED_OFF_SIG = APP_LED_OFF_SIG,
    LED_TIMEOUT_SIG = APP_MAX_SIG
};

static void Led_ctor(Led* me);

static QState Led_init(Led * const me, QEvt const * const e);

static QState Led_off(Led * const me, QEvt const * const e);
static QState Led_on(Led * const me, QEvt const * const e);

static QState Led_blinking(Led * const me, QEvt const * const e);
static QState Led_blinking_on(Led * const me, QEvt const * const e);
static QState Led_blinking_off(Led * const me, QEvt const * const e);

#endif /* _LED_AO_H */