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

#ifndef _BLINKY_H
#define _BLINKY_H

#include "qpc.h"

/* Define the event signals used in the application */
enum BlinkySignals {
    TIMEOUT_SIG = Q_USER_SIG, /* Offset the first signal by Q_USER_SIG */
    /* ... */
    MAX_SIG                   /* keep last (the number of signals) */
};

/* Active object(s) used in this application */
extern QActive * const AO_Blinky; /* opaque pointer to the Blinky AO */

void Blinky_ctor(void);

#endif /* _BLINKY_H */
