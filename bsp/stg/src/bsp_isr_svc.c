#include "bsp_isr_svc.h"
#include "bsp_qpc.h"

/* This function handles System service call via SWI instruction. */
void SVC_Handler(void) {
  QK_ISR_ENTRY();
  /* Not implemented */
  QK_ISR_EXIT();
}
