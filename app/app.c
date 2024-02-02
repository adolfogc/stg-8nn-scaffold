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

#include "app.h"
#include "led_ao.h"
#include "uavcan_node_ao.h"

int main(void);

static int App_mainDefault(void);

int App_main(void) __attribute__((weak, alias("App_mainDefault")));

typedef union {
  void *size_min;
  QEvt size_a;
  /* other event types follow here */
  LedEvt size_b;
} AppEvt;

static int App_mainDefault(void) {

  static QSubscrList subscrSto[APP_MAX_SIG];
  static AppEvt appEventSto[10U];

  static QEvt const * led_evtBuffer[10U];
  static QEvt const * uavcan_evtBuffer[10U];

  /* Initialize the QF framework and the underlying RT kernel */
  QF_init();
  /* Initialize the hardware */
  BSP_init();
  /* Initialize publish-subscribe */
  QF_psInit(subscrSto, Q_DIM(subscrSto));
  /* Initialize memory pools */
  QF_poolInit(appEventSto, sizeof(appEventSto), sizeof(appEventSto[0]));

  /* Start the active objects */

  QTicker_ctor((QTicker*)AO_ticker0, 0U);
  QACTIVE_START(AO_ticker0, Q_PRIO(1U, 1U), 0U, 0U, (void *)0, 0U, (void *)0);

  Led_ctor(AO_led);
  QACTIVE_START(AO_led, Q_PRIO(2U, 2U), led_evtBuffer, Q_DIM(led_evtBuffer), (void *)0, 0U, (void *)0);

  UavcanNode_ctor(AO_uavcanNode);
  QACTIVE_START(AO_uavcanNode, Q_PRIO(3U, 3U), uavcan_evtBuffer, Q_DIM(uavcan_evtBuffer), (void *)0, 0U, (void *)0);

  return QF_run();
}

int main(void) { return App_main(); }
