//$file${.::led::led_ao_private.h} vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
//
// Model: app.qm
// File:  ${.::led::led_ao_private.h}
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
//$endhead${.::led::led_ao_private.h} ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
#ifndef _LED_AO_PRIVATE_H
#define _LED_AO_PRIVATE_H

#include "app.h"

enum LedSignals {
  LED_ON_SIG = APP_LED_ON_SIG,
  LED_OFF_SIG = APP_LED_OFF_SIG,
  LED_BLINK_SIG = APP_LED_BLINK_SIG,
  TIMEOUT_SIG = APP_LED_TIMEOUT_SIG
};


#define LED_DEFAULT_BLINK_TIMEOUT (BSP_TICKS_PER_MS * APP_LED_DEFAULT_BLINK_PERIOD_MS)

#endif // _LED_AO_PRIVATE_H
