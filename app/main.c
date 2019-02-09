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

#include "bsp.h"
#include "main.h"
#include "led.h"
#include "uavcan_node.h"

/* -- Event queue buffers for the different active objects -- */
static QEvt const * g_led_queueBuffer[10];
static QEvt const * g_uavcanNode_queueBuffer[20];

/* -- Main loop -- */
int main(void)
{
    BSP_init(); /* Initialize the hardware. */
    QF_init();  /* Initialize the QF framework and the underlying RT kernel. */

    /* Inititalize the CAN hardware for use with Libcanard */
    BSP_Led_on();
    BSP_CAN_init();
    BSP_Led_off();

    /* -- Active objects -- */
    Led* led = initLedAO();
    UavcanNode* uavcanNode = initUavcanNode();

    /* Start the UavcanNode active object */
    QACTIVE_START((QActive*)&uavcanNode->super,
      1U,
      g_uavcanNode_queueBuffer,
      Q_DIM(g_uavcanNode_queueBuffer),
      (void*)0, 0U,
      (QEvt*)0);

    /* Start the Led active object */
    QACTIVE_START((QActive*)&led->super,
      2U,
      g_led_queueBuffer,
      Q_DIM(g_led_queueBuffer),
      (void*)0, 0U,
      (QEvt*)0);

    return QF_run();
}