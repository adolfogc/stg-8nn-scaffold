#ifndef _BSP_CAN_H
#define _BSP_CAN_H

#include "stm32f0xx_hal.h"

extern CAN_HandleTypeDef hcan;

void BSP_CAN_Init(void);

#endif /* _BSP_CAN_H */