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

int main(void);

static int App_mainDefault(void);
int App_main(void) __attribute__((weak, alias("App_mainDefault")));

static int App_mainDefault(void)
{
    static QSubscrList subscrSto[APP_MAX_SIG];

    static union AppEvent {
      void* size_min;
      QEvt size_a;
      /* other event types follow here */
    } appEventSto[10U];

    /* Initialize the AOs */
    BSP_Ticker0_initAO(); /* This AO is a singleton managed by its module. */
    UavcanNode_initAO();  /* This AO is a singleton managed by its module. */
    Led_initAO();         /* This AO is a singleton managed by its module. */

    /* Initialize the hardware. */
    BSP_init();
    /* Initialize the QF framework and the underlying RT kernel. */
    QF_init();
    /* Initialize publish-subscribe */
    QF_psInit(subscrSto, Q_DIM(subscrSto));
    /* Initialize memory pools */
    QF_poolInit(appEventSto, sizeof(appEventSto), sizeof(appEventSto[0]));

    /* Start the AOs */
    BSP_Ticker0_startAO(1U);
    Led_startAO(2U);
    UavcanNode_startAO(3U);

    return QF_run();
}

int main(void)
{
  return App_main();
}
