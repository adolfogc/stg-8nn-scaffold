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

#include <stdio.h>
#include <stdlib.h>
#include "bsp_specific.h"
#include "qpc.h"

Q_DEFINE_THIS_FILE

static volatile QTicker * l_ticker0Ptr = NULL;
static volatile QTicker l_ticker0;

void QF_onStartup(void)
{
    QF_setTickRate(BSP_TICKS_PER_SEC, 1);
    QF_consoleSetup();
}

void QF_onCleanup(void)
{
    QF_consoleCleanup();
}

void QF_onClockTick(void)
{
    if(l_ticker0Ptr != NULL) {
      QACTIVE_POST((QTicker*)l_ticker0Ptr, 0U, (void *)0);
    } else {
      QF_TICK_X(0U, (void *)0);
    }
}

void BSP_Ticker0_initAO(void)
{
    QTicker_ctor((QTicker*)&l_ticker0, 0U);
    l_ticker0Ptr = &l_ticker0;
}

void BSP_Ticker0_startAO(uint8_t priority)
{
    QACTIVE_START((QActive*)&l_ticker0,
      priority,
      (void*)0,
      0U,
      (void*)0, 0U,
      (QEvt*)0);
}

QTicker* BSP_Ticker0_getAO(void)
{
    return (QTicker*)&l_ticker0;
}

void Q_onAssert(char const * const module, int loc)
{
    fprintf(stderr, "Assertion failed in %s:%d\n", module, loc);
    QF_onCleanup();
    exit(-1);
}
