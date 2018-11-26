#ifndef _BSP_RTC_H
#define _BSP_RTC_H

#include "stm32f0xx_hal.h"

extern RTC_HandleTypeDef hrtc;

void BSP_RTC_Init(void);

#endif /* _BSP_RTC_H */