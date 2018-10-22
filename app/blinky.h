#ifndef BLINKY_H
#define BLINKY_H

#include <qpc.h>

/* define the event signals used in the application ------------------------*/
enum BlinkySignals {
    TIMEOUT_SIG = Q_USER_SIG, /* offset the first signal by Q_USER_SIG */
    MAX_SIG /* keep last (the number of signals) */
};

/* active object(s) used in this application -------------------------------*/
extern QActive * const AO_Blinky; /* opaque pointer to the Blinky AO */


void Blinky_ctor(void);

#endif // BLINKY_H
