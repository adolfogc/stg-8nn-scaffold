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

/* -- Active objects -- */
Led AO_led;
UavcanNode AO_uavcanNode;

/* -- Event queue buffers for the different active objects -- */
static QEvent const * g_led_queueBuffer[10];
static QEvent const * g_uavcanNode_queueBuffer[20];

/* -- Main loop -- */
int main(void)
{
    QF_init();  /* Initialize the QF framework and the underlying RT kernel. */
    BSP_init(); /* Initialize the hardware. */

    /* Inititalize the CAN hardware for use with Libcanard */
    BSP_Led_on();
    BSP_CAN_init();
    BSP_Led_off();

    /* Initialize our Libcanard's instance */
    APP_Canard_initInstance();

    /* Instantiate and start the UavcanNode active object */
    UavcanNode_ctor(&AO_uavcanNode);
    QACTIVE_START(&(AO_uavcanNode.super),
      1U,
      g_uavcanNode_queueBuffer,
      Q_DIM(g_uavcanNode_queueBuffer),
      (void*)0, 0U,
      (QEvent*)0);

    /* Instantiate and start the Led active object */
    Led_ctor(&AO_led);
    QACTIVE_START(&(AO_led.super),
      2U,
      g_led_queueBuffer,
      Q_DIM(g_led_queueBuffer),
      (void*)0, 0U,
      (QEvent*)0);

    return QF_run();
}