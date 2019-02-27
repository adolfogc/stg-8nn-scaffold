#include "bsp_qpc.h"
#include "bsp_isr_cec_can.h"

void CEC_CAN_IRQHandler(void)
{
  QK_ISR_ENTRY();
  QK_ISR_EXIT();
}
