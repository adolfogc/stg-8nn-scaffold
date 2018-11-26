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
#include "qpc.h"

void QF_onStartup(void)
{
  /* Not implemented yet */
}

void QF_onCleanup(void)
{
  /* Not implemented yet */
}

void QK_onIdle(void)
{
  /* Not implemented yet */
}

void Q_onAssert(char const * module, int loc)
{
  /* Information (module and loc) are not used yet */
  NVIC_SystemReset(); /* This should be called last */
}