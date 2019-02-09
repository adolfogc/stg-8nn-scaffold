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

void QF_onStartup(void)
{
    QF_setTickRate(BSP_TICKS_PER_SEC, 30);
    QF_consoleSetup();
}

void QF_onCleanup(void)
{
    QF_consoleCleanup();
}

void QF_onClockTick(void)
{
    int ch;
    QF_TICK_X(0U, &l_clock_tick); /* QF clock tick processing for rate 0 */
    ch = QF_consoleGetKey();
    if (ch != 0) { /* any key pressed? */
        //TODO: BSP_onKeyboardInput(ch);
    }
}

void Q_onAssert(char const * const module, int loc)
{
    fprintf(stderr, "Assertion failed in %s:%d\n", module, loc);
    QF_onCleanup();
    exit(-1);
}