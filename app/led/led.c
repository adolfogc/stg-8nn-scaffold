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

#include "led.h"
#include "app.h"
#include "led_ao.h"

Q_DEFINE_THIS_FILE

/* -- Active object implementation -- */

#include "led_ao.c"

/* -- Active object event queue -- */

static QEvt const *l_led_evtBuffer[APP_LED_EVT_QUEUE_SIZE];

/* -- Public functions implementation -- */

void Led_initAO(void) { Led_ctor(); }

void Led_startAO(uint8_t priority) {
  QACTIVE_START(AO_led, priority, l_led_evtBuffer, Q_DIM(l_led_evtBuffer), (void *)0, 0U, (QEvt *)0);
}
