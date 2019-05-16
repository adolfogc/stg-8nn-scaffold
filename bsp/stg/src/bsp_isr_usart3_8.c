#include "bsp_isr_usart3_8.h"
#include "bsp_qpc.h"

/* This function handles USART3 to USART8 global interrupts / USART3 wake-up interrupt through EXTI line 28. */
void USART3_8_IRQHandler(void) {
  QK_ISR_ENTRY();
  QK_ISR_EXIT();
}
