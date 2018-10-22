#include <qpc.h>
#include <bsp.h>
#include "blinky.h"

int main(void) {
  static QEvt const *blinky_queueSto[10]; /* event queue buffer for Blinky */

  QF_init();
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
