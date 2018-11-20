/*
* Copyright (C) 2018 Adolfo E. García
*
* This file is part of STG-8nn-Scaffold.
*
* STG-8nn-Scaffold is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* STG-8nn-Scaffold is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with STG-8nn-Scaffold.  If not, see <www.gnu.org/licenses/>.
*/

#include <qpc.h>
#include <bsp.h>
#include "blinky.h"

int main(void) {
  static QEvt const *blinky_queueSto[10]; /* event queue buffer for Blinky */

  QF_init();

  /* Initialize the Board Support Package
   * Note: BSP_init() has to be called *after* initializing publish-subscribe and
   * event pools, to make the system ready to accept SysTick interrupts.
   * Unfortunately, the STM32Cube code that must be called from the BSP,
   * configures and starts SysTick.
  */
  BSP_init();

  /* instantiate and start the Blinky active object */
  Blinky_ctor();
  QACTIVE_START(AO_Blinky,  /* active object to start */
    1U,                     /* priority of the active object */
    blinky_queueSto,        /* event queue buffer */
    Q_DIM(blinky_queueSto), /* the length of the buffer */
    (void *)0, 0U,          /* private stack (not used) */
    (QEvt *)0);             /* initialization event (not used) */

  return QF_run(); /* let the framework run the application */
}
