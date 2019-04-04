/*
 * Copyright (c) 2009-2019 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 /*
  * STG-8nn-Scaffold
  * This is a derived version by Adolfo E. García (2019).
  */

#include <stdint.h>
#include "stm32f0xx.h"
#include "app.h"

/* -- Linker generated Symbols -- */
extern uint32_t __etext;
extern uint32_t __data_start__;
extern uint32_t __data_end__;
extern uint32_t __copy_table_start__;
extern uint32_t __copy_table_end__;
extern uint32_t __zero_table_start__;
extern uint32_t __zero_table_end__;
extern uint32_t __bss_start__;
extern uint32_t __bss_end__;
extern uint32_t __StackTop;

/* -- Exception / Interrupt Handler Function Prototype -- */
typedef void( *pFunc )( void );

/* -- External References -- */
extern void _start(void)   __attribute__((noreturn)); /* PreeMain (C library entry point) */

/* -- Internal References -- */
void Default_Handler(void) __attribute__ ((noreturn));
void Reset_Handler(void)   __attribute__ ((noreturn));

/* -- User Initial Stack & Heap -- */

/* Stack Size (in Bytes) <0x0-0xFFFFFFFF:8> */
#define  __STACK_SIZE  APP_STACK_SIZE
static uint8_t stack[__STACK_SIZE] __attribute__ ((aligned(8), used, section(".stack")));

/* Heap Size (in Bytes) <0x0-0xFFFFFFFF:8> */
#define  __HEAP_SIZE   APP_HEAP_SIZE
#if __HEAP_SIZE > 0
static uint8_t heap[__HEAP_SIZE]   __attribute__ ((aligned(8), used, section(".heap")));
#endif

/* -- Exception / Interrupt Handler -- */

/* Exceptions */
void NMI_Handler                      (void) __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler                (void) __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler                      (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler                  (void) __attribute__ ((weak, alias("Default_Handler")));

void WWDG_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void PVD_VDDIO2_IRQHandler            (void) __attribute__ ((weak, alias("Default_Handler")));
void RTC_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void FLASH_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void RCC_CRS_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI0_1_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI2_3_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void EXTI4_15_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler")));
void TSC_IRQHandler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Ch1_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler (void) __attribute__ ((weak, alias("Default_Handler")));
void ADC1_COMP_IRQHandler             (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_BRK_UP_TRG_COM_IRQHandler   (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM1_CC_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM3_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM6_DAC_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM7_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM14_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM15_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM16_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void TIM17_IRQHandler                 (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void I2C2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void SPI2_IRQHandler                  (void) __attribute__ ((weak, alias("Default_Handler")));
void USART1_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void USART2_IRQHandler                (void) __attribute__ ((weak, alias("Default_Handler")));
void USART3_8_IRQHandler              (void) __attribute__ ((weak, alias("Default_Handler")));
void CEC_CAN_IRQHandler               (void) __attribute__ ((weak, alias("Default_Handler")));

/* -- Exception / Interrupt Vector table -- */
extern const pFunc __Vectors[ 48];
       const pFunc __Vectors[ 48] __attribute__((used, section(".vectors"))) = {
  (pFunc)(&__StackTop),                     /*     Initial Stack Pointer */
  Reset_Handler,                            /*     Reset Handler */
  NMI_Handler,                              /* -14 NMI Handler */
  HardFault_Handler,                        /* -13 Hard Fault Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  SVC_Handler,                              /*  -5 SVCall Handler */
  0,                                        /*     Reserved */
  0,                                        /*     Reserved */
  PendSV_Handler,                           /*  -2 PendSV Handler */
  SysTick_Handler,                          /*  -1 SysTick Handler */

  /* Interrupts */
  WWDG_IRQHandler,                          /*   0 Window WatchDog */
  PVD_VDDIO2_IRQHandler,                    /*   1 PVD and VDDIO2 through EXTI Line detect */
  RTC_IRQHandler,                           /*   2 RTC through the EXTI line */
  FLASH_IRQHandler,                         /*   3 FLASH */
  RCC_CRS_IRQHandler,                       /*   4 RCC and CRS */
  EXTI0_1_IRQHandler,                       /*   5 EXTI Line 0 and 1 */
  EXTI2_3_IRQHandler,                       /*   6 EXTI Line 2 and 3 */
  EXTI4_15_IRQHandler,                      /*   7 EXTI Line 4 to 15 */
  TSC_IRQHandler,                           /*   8 TSC */
  DMA1_Ch1_IRQHandler,                      /*   9 DMA1 Channel 1 */
  DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler,         /*  10 DMA1 Channel 2 and 3 & DMA2 Channel 1 and 2 */
  DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler,         /*  11 DMA1 Channel 4 to 7 & DMA2 Channel 3 to 5 */
  ADC1_COMP_IRQHandler,                     /*  12 ADC1, COMP1 and COMP2 */
  TIM1_BRK_UP_TRG_COM_IRQHandler,           /*  13 TIM1 Break, Update, Trigger and Commutation */
  TIM1_CC_IRQHandler,                       /*  14 TIM1 Capture Compare */
  TIM2_IRQHandler,                          /*  15 TIM2 */
  TIM3_IRQHandler,                          /*  16 TIM3 */
  TIM6_DAC_IRQHandler,                      /*  17 TIM6 and DAC */
  TIM7_IRQHandler,                          /*  18 TIM7 */
  TIM14_IRQHandler,                         /*  19 TIM14 */
  TIM15_IRQHandler,                         /*  20 TIM15 */
  TIM16_IRQHandler,                         /*  21 TIM16 */
  TIM17_IRQHandler,                         /*  22 TIM17 */
  I2C1_IRQHandler,                          /*  23 I2C1 */
  I2C2_IRQHandler,                          /*  24 I2C2 */
  SPI1_IRQHandler,                          /*  25 SPI1 */
  SPI2_IRQHandler,                          /*  26 SPI2 */
  USART1_IRQHandler,                        /*  27 USART1 */
  USART2_IRQHandler,                        /*  28 USART2 */
  USART3_8_IRQHandler,                      /*  29 USART3, USART4, USART5, USART6, USART7, USART8 */
  CEC_CAN_IRQHandler                        /*  30 CEC and CAN */
};

/* -- Reset Handler called on controller reset -- */
void Reset_Handler(void)
{
  uint32_t *pSrc, *pDest;
  uint32_t *pTable __attribute__((unused));

  SystemInit(); /* CMSIS System Initialization */

  /* Copy data from read only memory to RAM.
   * The ranges of copy from/to are specified by following symbols:
   *   __etext, LMA of start of the section to copy from. Usually end of text
   *   __data_start__, VMA of start of the section to copy to
   *   __data_end__, VMA of end of the section to copy to
   *
   * All addresses must be aligned to 4 bytes boundary.
   */
  pSrc  = &__etext;
  pDest = &__data_start__;

  for ( ; pDest < &__data_end__ ; ) {
    *pDest++ = *pSrc++;
  }

  /* Clear the BSS section.
   *
   * The BSS section is specified by following symbols
   *   __bss_start__: start of the BSS section.
   *   __bss_end__: end of the BSS section.
   *
   * Both addresses must be aligned to 4 bytes boundary.
   */
  pDest = &__bss_start__;

  for ( ; pDest < &__bss_end__ ; ) {
    *pDest++ = 0UL;
  }

  _start(); /* Enter PreeMain (C library entry point) */
}

/* -- Default Handler for Exceptions / Interrupts -- */
void Default_Handler(void)
{
  while(1);
}
