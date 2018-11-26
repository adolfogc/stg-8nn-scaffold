#include "bsp/tim.h"
#include "bsp/gpio.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"

Q_DEFINE_THIS_FILE

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim16;
TIM_HandleTypeDef htim17;

void BSP_TIM1_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 0;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  Q_ENSURE(HAL_TIM_Base_Init(&htim1) == HAL_OK);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  Q_ENSURE(HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) == HAL_OK);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  Q_ENSURE(HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) == HAL_OK);
}

void BSP_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_IC_InitTypeDef sConfigIC;

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 3;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  Q_ENSURE(HAL_TIM_Base_Init(&htim2) == HAL_OK);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  Q_ENSURE(HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) == HAL_OK);
  Q_ENSURE(HAL_TIM_IC_Init(&htim2) == HAL_OK);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  Q_ENSURE(HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) == HAL_OK);

  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  Q_ENSURE(HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_1) == HAL_OK);

  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  Q_ENSURE(HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_2) == HAL_OK);
}

void BSP_TIM16_Init(void)
{
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 0;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 0;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  Q_ENSURE(HAL_TIM_Base_Init(&htim16) == HAL_OK);
}

void BSP_TIM17_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;

  htim17.Instance = TIM17;
  htim17.Init.Prescaler = 15;
  htim17.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim17.Init.Period = 999;
  htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim17.Init.RepetitionCounter = 0;
  htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  Q_ENSURE(HAL_TIM_Base_Init(&htim17) == HAL_OK);
  Q_ENSURE(HAL_TIM_PWM_Init(&htim17) == HAL_OK);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 500;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  Q_ENSURE(HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) == HAL_OK);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  Q_ENSURE(HAL_TIMEx_ConfigBreakDeadTime(&htim17, &sBreakDeadTimeConfig) == HAL_OK);

  HAL_TIM_MspPostInit(&htim17);
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(tim_baseHandle->Instance == TIM1)
  {
    /* Enable TIM1 clock */
    __HAL_RCC_TIM1_CLK_ENABLE();
  
    /* -- TIM1 GPIO configuration --    
       PA12 ------> TIM1_ETR 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(tim_baseHandle->Instance == TIM2)
  {
    /* Enable TIM2 clock */
    __HAL_RCC_TIM2_CLK_ENABLE();
  
    /* -- TIM2 GPIO configuration --    
       PA15 ------> TIM2_CH1 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_15;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
  else if(tim_baseHandle->Instance == TIM16)
  {
    /* Enable TIM16 clock */
    __HAL_RCC_TIM16_CLK_ENABLE();
  }
  else if(tim_baseHandle->Instance == TIM17)
  {
    /* Enable TIM17 clock */
    __HAL_RCC_TIM17_CLK_ENABLE();
  }
}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(timHandle->Instance == TIM17)
  {
    /* -- TIM17 GPIO configuration --
       PB7 ------> TIM17_CH1N 
    */
    GPIO_InitStruct.Pin = Out9_LS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF2_TIM17;
    HAL_GPIO_Init(Out9_LS_GPIO_Port, &GPIO_InitStruct);
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{
  if(tim_baseHandle->Instance == TIM1)
  {
    /* Disable peripheral clock */
    __HAL_RCC_TIM1_CLK_DISABLE();
  
    /* -- TIM1 GPIO configuration --
       PA12 ------> TIM1_ETR 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_12);
  }
  else if(tim_baseHandle->Instance == TIM2)
  {
    /* Disable peripheral clock */
    __HAL_RCC_TIM2_CLK_DISABLE();
  
    /* -- TIM2 GPIO Configuration --
       PA15 ------> TIM2_CH1 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_15);
  }
  else if(tim_baseHandle->Instance == TIM16)
  {
    /* Disable peripheral clock */
    __HAL_RCC_TIM16_CLK_DISABLE();
  }
  else if(tim_baseHandle->Instance == TIM17)
  {
    /* Disable peripheral clock */
    __HAL_RCC_TIM17_CLK_DISABLE();
  }
}