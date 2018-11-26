#include "bsp/usart.h"
#include "bsp/gpio.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"
#include "bsp/irs_pri.h"

Q_DEFINE_THIS_FILE

UART_HandleTypeDef huart1;
IRDA_HandleTypeDef hirda3;

void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  Q_ENSURE(HAL_UART_Init(&huart1) == HAL_OK);
}

void MX_USART3_IRDA_Init(void)
{
  hirda3.Instance = USART3;
  hirda3.Init.BaudRate = 115200;
  hirda3.Init.WordLength = IRDA_WORDLENGTH_8B;
  hirda3.Init.Parity = IRDA_PARITY_NONE;
  hirda3.Init.Mode = IRDA_MODE_TX_RX;
  hirda3.Init.Prescaler = 10;
  hirda3.Init.PowerMode = IRDA_POWERMODE_NORMAL;
  Q_ENSURE(HAL_IRDA_Init(&hirda3) == HAL_OK);
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(uartHandle->Instance == USART1)
  {
    /* Enable USART1 clock */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    /* -- USART1 GPIO configuration --    
       PA9  ------> USART1_TX
       PA10 ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_IRDA_MspInit(IRDA_HandleTypeDef* irdaHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(irdaHandle->Instance == USART3)
  {
    /* Enable USART3 clock */
    __HAL_RCC_USART3_CLK_ENABLE();
  
    /* -- USART3 GPIO configuration --  
       PB10 ------> USART3_TX
       PB11 ------> USART3_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_10|GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* USART3 interrupt Init */
    HAL_NVIC_SetPriority(USART3_8_IRQn, BSP_USART3_PRIO, 0);
    HAL_NVIC_EnableIRQ(USART3_8_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{
  if(uartHandle->Instance == USART1)
  {
    /* Disable peripheral clock */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /* -- USART1 GPIO configuration --   
       PA9  ------> USART1_TX
       PA10 ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
  }
}

void HAL_IRDA_MspDeInit(IRDA_HandleTypeDef* irdaHandle)
{
  if(irdaHandle->Instance == USART3)
  {
    /* Disable peripheral clock */
    __HAL_RCC_USART3_CLK_DISABLE();
  
    /* USART3 GPIO Configuration    
       PB10 ------> USART3_TX
       PB11 ------> USART3_RX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10|GPIO_PIN_11);

    /* Deinit USART3 interrupt */
    HAL_NVIC_DisableIRQ(USART3_8_IRQn);
  }
}