#include "bsp/i2c.h"
#include "bsp/gpio.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"

Q_DEFINE_THIS_FILE

I2C_HandleTypeDef hi2c2;

void BSP_I2C2_Init(void)
{
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  Q_ENSURE(HAL_I2C_Init(&hi2c2) == HAL_OK);

  /* Configure analogue filter */
  Q_ENSURE(HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) == HAL_OK);

  /* Configure digital filter */
  Q_ENSURE(HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) == HAL_OK);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  if(i2cHandle->Instance == I2C2)
  {
    /* -- I2C2 GPIO Configuration --    
       PB13 ------> I2C2_SCL
       PB14 ------> I2C2_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF5_I2C2;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    /* I2C2 clock enable */
    __HAL_RCC_I2C2_CLK_ENABLE();
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{
  if(i2cHandle->Instance == I2C2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_I2C2_CLK_DISABLE();
    /* -- I2C2 GPIO Configuration --    
       PB13 ------> I2C2_SCL
       PB14 ------> I2C2_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_13|GPIO_PIN_14);
  }
}