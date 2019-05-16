#include "bsp_isr_hardfault.h"

__attribute__((noreturn)) void HardFault_Handler(void) {
  while (1) {
    /* Do nothing */
  }
}
