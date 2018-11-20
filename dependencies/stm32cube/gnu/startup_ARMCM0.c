/* File: startup_ARMCM0.c
 * Purpose: startup file for Cortex-M0 devices.
 *          Should be used with GCC 'GNU Tools ARM Embedded'
 * Version: V1.01
 * Date: 12 June 2014
 *
 */
/* Copyright (c) 2011 - 2014 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

#include <stdint.h>

/* start and end of stack defined in the linker script ---------------------*/
extern int __stack_start__;
extern int __stack_end__;

/* Weak prototypes for error handlers --------------------------------------*/
/**
* The function assert_failed defined at the end of this file defines
* the error/assertion handling policy for the application and might
* need to be customized for each project. This function is defined in
* assembly to avoid accessing the stack, which might be corrupted by
* the time assert_failed is called.
*/
__attribute__ ((naked)) void assert_failed(char const *module, int loc);

/*----------------------------------------------------------------------------
  Prototypes
 *----------------------------------------------------------------------------*/
__attribute__ ((naked)) void Default_Handler(void);                   /* Default empty handler */
__attribute__ ((naked)) void Reset_Handler(void);                     /* Reset Handler */
__attribute__ ((naked)) void Q_onAssert(char const *module, int loc); /* QP assertion handler */
void SystemInit(void);                                                /* CMSIS system initialization */

/*----------------------------------------------------------------------------
  Exception / Interrupt Handler
 *----------------------------------------------------------------------------*/
/* Cortex-M0 Processor Exceptions */
/* Fault */
void NMI_Handler                      (void) __attribute__ ((weak));
void HardFault_Handler                (void) __attribute__ ((weak));
/* Non-fault */
void SVC_Handler                      (void) __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler                   (void) __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler                  (void) __attribute__ ((weak, alias("Default_Handler")));

/* External Interrupts */
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

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
__attribute__ ((section(".isr_vector")))
int const g_pfnVectors[] = {
  /* Cortex-M0 Exceptions Handler */
  (int)&__stack_end__,                      /*      Initial Stack Pointer                          */
  (int)&Reset_Handler,                      /*      Reset Handler                                  */
  (int)&NMI_Handler,                        /*      NMI Handler                                    */
  (int)&HardFault_Handler,                  /*      Hard Fault Handler                             */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  (int)&SVC_Handler,                        /*      SVCall Handler                                 */
  0,                                        /*      Reserved                                       */
  0,                                        /*      Reserved                                       */
  (int)&PendSV_Handler,                     /*      PendSV Handler                                 */
  (int)&SysTick_Handler,                    /*      SysTick Handler                                */
  /* External interrupts */
  (int)&WWDG_IRQHandler,                    /*  0:  Watchdog Timer                                 */
  (int)&PVD_VDDIO2_IRQHandler,              /*  1:  PVD through EXTI Line detect                   */
  (int)&RTC_IRQHandler,                     /*  2:  RTC through EXTI Line                          */
  (int)&FLASH_IRQHandler,                   /*  3:  FLASH                                          */
  (int)&RCC_CRS_IRQHandler,                 /*  4:  RCC and CRS                                    */
  (int)&EXTI0_1_IRQHandler,                 /*  5:  EXTI Line 0 and 1                              */
  (int)&EXTI2_3_IRQHandler,                 /*  6:  EXTI Line 2 and 3                              */
  (int)&EXTI4_15_IRQHandler,                /*  7:  EXTI Line 4 to 15                              */
  (int)&TSC_IRQHandler,                     /*  8:  TS                                             */
  (int)&DMA1_Ch1_IRQHandler,                /*  9:  DMA1 Channel 1                                 */
  (int)&DMA1_Ch2_3_DMA2_Ch1_2_IRQHandler,   /* 10:  DMA1 Channel 2 and 3 & DMA2 Channel 1 and 2    */
  (int)&DMA1_Ch4_7_DMA2_Ch3_5_IRQHandler,   /* 11:  DMA1 Channel 4 to 7 & DMA2 Channel 3 to 5      */
  (int)&ADC1_COMP_IRQHandler,               /* 12:  ADC1, COMP1 and COMP2                          */
  (int)&TIM1_BRK_UP_TRG_COM_IRQHandler,     /* 13:  TIM1 Break, Update, Trigger and Commutation    */
  (int)&TIM1_CC_IRQHandler,                 /* 14:  TIM1 Capture Compare                           */
  (int)&TIM2_IRQHandler,                    /* 15:  TIM2                                           */
  (int)&TIM3_IRQHandler,                    /* 16:  TIM3                                           */
  (int)&TIM6_DAC_IRQHandler,                /* 17:  TIM6 and DAC                                   */
  (int)&TIM7_IRQHandler,                    /* 18:  TIM7                                           */
  (int)&TIM14_IRQHandler,                   /* 19:  TIM14                                          */
  (int)&TIM15_IRQHandler,                   /* 20:  TIM15                                          */
  (int)&TIM16_IRQHandler,                   /* 21:  TIM16                                          */
  (int)&TIM17_IRQHandler,                   /* 22:  TIM17                                          */
  (int)&I2C1_IRQHandler,                    /* 23:  I2C1                                           */
  (int)&I2C2_IRQHandler,                    /* 24:  I2C2                                           */
  (int)&SPI1_IRQHandler,                    /* 25:  SPI1                                           */
  (int)&SPI2_IRQHandler,                    /* 26:  SPI2                                           */
  (int)&USART1_IRQHandler,                  /* 27:  USART1                                         */
  (int)&USART2_IRQHandler,                  /* 28:  USART2                                         */
  (int)&USART3_8_IRQHandler,                /* 29:  USART3, USART4, USART5, USART6, USART7, USART8 */
  (int)&CEC_CAN_IRQHandler,                 /* 30:  CEC and CAN                                    */
  0                                         /* 31:  Not used                                       */
};

/*----------------------------------------------------------------------------
  Reset Handler called on controller reset
 *----------------------------------------------------------------------------*/
void Reset_Handler(void) {
  extern int main(void);
  extern int __libc_init_array(void);
  extern unsigned __data_start;  /* start of .data in the linker script */
  extern unsigned __data_end__;  /* end of .data in the linker script */
  extern unsigned const __data_load; /* initialization values for .data  */
  extern unsigned __bss_start__; /* start of .bss in the linker script */
  extern unsigned __bss_end__;   /* end of .bss in the linker script */
  extern void software_init_hook(void) __attribute__((weak));

  unsigned const *src;
  unsigned *dst;

  SystemInit(); /* CMSIS system initialization */

  /* copy the data segment initializers from flash to RAM... */
  src = &__data_load;
  for (dst = &__data_start; dst < &__data_end__; ++dst, ++src) {
      *dst = *src;
  }

  /* zero fill the .bss segment in RAM... */
  for (dst = &__bss_start__; dst < &__bss_end__; ++dst) {
      *dst = 0;
  }

  /* init hook provided? */
  if (&software_init_hook != (void (*)(void))(0)) {
      /* give control to the RTOS */
      software_init_hook(); /* this will also call __libc_init_array */
  }
  else {
      /* call all static constructors in C++ (comment out in C programs) */
      //__libc_init_array();
      (void)main(); /* application's entry point; should never return! */
  }

  /* the previous code should not return, but assert just in case... */
  assert_failed("Reset_Handler", __LINE__);
}

/*----------------------------------------------------------------------------
  Fault Exception Handlers
 *----------------------------------------------------------------------------*/
__attribute__ ((naked))
void NMI_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_nmi\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_nmi: .asciz \"NMI\"\n\t"
        "  .align 2\n\t"
    );
}

__attribute__ ((naked))
void HardFault_Handler(void) {
    __asm volatile (
        "    ldr r0,=str_hrd\n\t"
        "    mov r1,#1\n\t"
        "    b assert_failed\n\t"
        "str_hrd: .asciz \"HardFault\"\n\t"
        "  .align 2\n\t"
    );
}

/*----------------------------------------------------------------------------
  Default Handler for Exceptions / Interrupts
 *----------------------------------------------------------------------------*/
__attribute__ ((naked))
 void Default_Handler(void) {
     __asm volatile (
         "    ldr r0,=str_dflt\n\t"
         "    mov r1,#1\n\t"
         "    b assert_failed\n\t"
         "str_dflt: .asciz \"Default\"\n\t"
         "  .align 2\n\t"
     );
 }

/*****************************************************************************
* The function assert_failed defines the error/assertion handling policy
* for the application. After making sure that the stack is OK, this function
* calls Q_onAssert, which should NOT return (typically reset the CPU).
*
* NOTE: the function Q_onAssert should NOT return.
*****************************************************************************/
__attribute__ ((naked))
void assert_failed(char const *module, int loc) {
    /* re-set the SP in case of stack overflow */
    __asm volatile (
        "  MOV sp,%0\n\t"
        : : "r" (&__stack_end__));

    Q_onAssert(module, loc); /* call the application-specific QP handler */

    for (;;) { /* should not be reached, but just in case loop forever... */
    }
}
