/**
 ******************************************************************************
 * File Name          : RTC.c
 * Description        : This file provides code for the configuration
 *                      of the RTC instances.
 ******************************************************************************
 ** This notice applies to any and all portions of this file
 * that are not between comment pairs USER CODE BEGIN and
 * USER CODE END. Other portions of this file, whether
 * inserted by the user or by software development tools
 * are owned by their respective copyright owners.
 *
 * COPYRIGHT(c) 2019 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#include "bsp_mx_rtc.h"
#include "stm32f0xx_ll_rcc.h"

/* RTC init function */
void BSP_MX_RTC_Init(void) {
  LL_RTC_InitTypeDef RTC_InitStruct;
  LL_RTC_TimeTypeDef RTC_TimeStruct;
  LL_RTC_DateTypeDef RTC_DateStruct;

  /* Peripheral clock enable */
  LL_RCC_EnableRTC();

  /* Initialize RTC and set the Time and Date */
  RTC_InitStruct.HourFormat = LL_RTC_HOURFORMAT_24HOUR;
  RTC_InitStruct.AsynchPrescaler = 127;
  RTC_InitStruct.SynchPrescaler = 255;
  LL_RTC_Init(RTC, &RTC_InitStruct);

  LL_RTC_SetAsynchPrescaler(RTC, 127);
  LL_RTC_SetSynchPrescaler(RTC, 255);

  /* Initialize RTC and set the Time and Date */
  if (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2) {
    RTC_TimeStruct.Hours = 0;
    RTC_TimeStruct.Minutes = 0;
    RTC_TimeStruct.Seconds = 0;
    LL_RTC_TIME_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_TimeStruct);
    RTC_DateStruct.WeekDay = LL_RTC_WEEKDAY_MONDAY;
    RTC_DateStruct.Month = LL_RTC_MONTH_JANUARY;
    RTC_DateStruct.Year = 0;
    LL_RTC_DATE_Init(RTC, LL_RTC_FORMAT_BCD, &RTC_DateStruct);
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0, 0x32F2);
  }
  /* Initialize RTC and set the Time and Date */
  if (LL_RTC_BAK_GetRegister(RTC, LL_RTC_BKP_DR0) != 0x32F2) {
    LL_RTC_BAK_SetRegister(RTC, LL_RTC_BKP_DR0, 0x32F2);
  }
}
