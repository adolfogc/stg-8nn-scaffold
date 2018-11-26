#include "bsp/rtc.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"

Q_DEFINE_THIS_FILE

RTC_HandleTypeDef hrtc;

/* RTC init function */
void BSP_RTC_Init(void)
{
  RTC_TimeTypeDef sTime;
  RTC_DateTypeDef sDate;

  /* Initialize only the RTC */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  Q_ENSURE(HAL_RTC_Init(&hrtc) == HAL_OK);

  /* Initialize the RTC and set the time and date */
  sTime.Hours = 0x0;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  Q_ENSURE(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) == HAL_OK);

  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  Q_ENSURE(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) == HAL_OK);

  /* Enable WakeUp */
  Q_ENSURE(HAL_RTCEx_SetWakeUpTimer(&hrtc, 0, RTC_WAKEUPCLOCK_RTCCLK_DIV16) == HAL_OK);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{
  if(rtcHandle->Instance == RTC)
  {
    /* Enable the RTC clock */
    __HAL_RCC_RTC_ENABLE();
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{
  if(rtcHandle->Instance == RTC)
  {
    /* Disable the RTC clock */
    __HAL_RCC_RTC_DISABLE();
  }
}