#include "bsp/can.h"
#include "bsp/gpio.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"
#include "bsp/irs_pri.h"

Q_DEFINE_THIS_FILE

CAN_HandleTypeDef hcan;

void BSP_CAN_Init(void)
{
  hcan.Instance = CAN;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SJW = CAN_SJW_1TQ;
  hcan.Init.BS1 = CAN_BS1_11TQ;
  hcan.Init.BS2 = CAN_BS2_4TQ;
  hcan.Init.TTCM = DISABLE;
  hcan.Init.ABOM = ENABLE;
  hcan.Init.AWUM = DISABLE;
  hcan.Init.NART = DISABLE;
  hcan.Init.RFLM = DISABLE;
  hcan.Init.TXFP = DISABLE;
  Q_ENSURE(HAL_CAN_Init(&hcan) == HAL_OK);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(canHandle->Instance==CAN)
  {
    /* CAN clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* CAN interrupt Init */
    HAL_NVIC_SetPriority(CEC_CAN_IRQn, BSP_CAN_PRIO, 0);
    HAL_NVIC_EnableIRQ(CEC_CAN_IRQn);
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN)
  {
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();
  
    /**CAN GPIO Configuration    
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN interrupt Deinit */
    HAL_NVIC_DisableIRQ(CEC_CAN_IRQn);
  }
}