/*
Copyright (C) 2018 Adolfo E. García

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

#include "bsp.h"
#include "qpc.h"
#include "main.h"
#include "blinky.h"

/* -- Check for the minimum required QP version -- */
#if (QP_VERSION < 630U) || (QP_VERSION != ((QP_RELEASE^4294967295U) % 0x3E8U))
#error qpc version 6.3.0 or higher required
#endif

/* -- Main loop -- */
int main(void)
{
  static QEvt const *blinky_queueSto[10]; /* Event queue buffer for Blinky */
  QF_init();
  Main_Init();

  /* Instantiate and start the Blinky active object */
  Blinky_ctor();
  QACTIVE_START(AO_Blinky,  /* Active object to start */
    1U,                     /* Priority of the active object */
    blinky_queueSto,        /* Event queue buffer */
    Q_DIM(blinky_queueSto), /* Length of the buffer */
    (void *)0, 0U,          /* Private stack (not used) */
    (QEvt *)0);             /* Initialization event (not used) */

  return QF_run();
}

/* -- Private functions -- */
static void Main_Init(void)
{
  HAL_Init();
  /* Configure the system clock */
  SystemClock_Config();
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ADC_Init();
  MX_CAN_Init();
  /* Note: Watchdog can be enabled by calling MX_IWDG_Init/0 here */  
  MX_RTC_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_USART1_UART_Init();
  MX_USART3_IRDA_Init();
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init(); 
}

static void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_PeriphCLKInitTypeDef PeriphClkInit;

  /* Initialize the CPU, AHB and APB buses' clocks */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI14|RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI14State = RCC_HSI14_ON;
  RCC_OscInitStruct.HSI14CalibrationValue = 16U;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART3|RCC_PERIPHCLK_USART1|RCC_PERIPHCLK_RTC;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  PeriphClkInit.Usart3ClockSelection = RCC_USART3CLKSOURCE_PCLK1;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_HSE_DIV32;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

  /* Configure the Systick interrupt
   * Note 0: The function HAL_SYSTICK_Config/1 already substracts one from the reload value, so don't substract one.
   * Note 1: The HAL drivers require that the systick interrupt occurrs every 1 ms.
  */
  const uint32_t u32Reload = HAL_RCC_GetHCLKFreq() / BSP_TICKS_PER_SEC;
  HAL_SYSTICK_Config(u32Reload);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, BSP_SYSTICK_PRIO, 0);
}

 /* -- Overrides --  */
void HAL_SYSTICK_Callback(void)
{
  /* Not implemented */
}

void _Error_Handler(char* file, int line)
{
  while(1)
  {
    /* Do nothing */
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{ 
}
#endif /* USE_FULL_ASSERT */