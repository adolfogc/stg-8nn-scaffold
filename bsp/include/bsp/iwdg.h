#ifndef _BSP_IWDG_H
#define _BSP_IWDG_H

#include "stm32f0xx_hal.h"

extern IWDG_HandleTypeDef hiwdg;

void MX_IWDG_Init(void);

#endif /* _BSP_IWDG_H */