#include "bsp/adc.h"
#include "bsp/gpio.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"

Q_DEFINE_THIS_FILE

ADC_HandleTypeDef hadc;

void MX_ADC_Init(void)
{
  ADC_ChannelConfTypeDef sConfig;

  /* Configure the global features of the ADC: clock, resolution, data alignment and number of conversion. */
  hadc.Instance = ADC1;
  hadc.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc.Init.Resolution = ADC_RESOLUTION_12B;
  hadc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  hadc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc.Init.LowPowerAutoWait = DISABLE;
  hadc.Init.LowPowerAutoPowerOff = DISABLE;
  hadc.Init.ContinuousConvMode = DISABLE;
  hadc.Init.DiscontinuousConvMode = DISABLE;
  hadc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc.Init.DMAContinuousRequests = DISABLE;
  hadc.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  Q_ENSURE(HAL_ADC_Init(&hadc) == HAL_OK);

  /* Configure, for the selected ADC, the regular channel to be converted. */
  sConfig.Channel = ADC_CHANNEL_0;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
  sConfig.SamplingTime = ADC_SAMPLETIME_1CYCLE_5;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  /* Configure, for the selected ADC, the regular channel to be converted. */
  sConfig.Channel = ADC_CHANNEL_1;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_2;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_3;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_5;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_6;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_VREFINT;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);

  sConfig.Channel = ADC_CHANNEL_VBAT;
  Q_ENSURE(HAL_ADC_ConfigChannel(&hadc, &sConfig) == HAL_OK);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  if(adcHandle->Instance == ADC1)
  {
    /* Enable ADC1 clock */
    __HAL_RCC_ADC1_CLK_ENABLE();
  
    /* -- ADC GPIO configuration --   
       PA0 ------> ADC_IN0
       PA1 ------> ADC_IN1
       PA2 ------> ADC_IN2
       PA3 ------> ADC_IN3
       PA5 ------> ADC_IN5
       PA6 ------> ADC_IN6 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{
  if(adcHandle->Instance==ADC1)
  {
    /* Disable peripheral clock */
    __HAL_RCC_ADC1_CLK_DISABLE();
  
    /* -- ADC GPIO Configuration -- 
       PA0 ------> ADC_IN0
       PA1 ------> ADC_IN1
       PA2 ------> ADC_IN2
       PA3 ------> ADC_IN3
       PA5 ------> ADC_IN5
       PA6 ------> ADC_IN6 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_5|GPIO_PIN_6);
  }
}