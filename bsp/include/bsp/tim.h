#ifndef _BSP_TIM_H
#define _BSP_TIM_H

#include "stm32f0xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim16;
extern TIM_HandleTypeDef htim17;

void BSP_TIM1_Init(void);
void BSP_TIM2_Init(void);
void BSP_TIM16_Init(void);
void BSP_TIM17_Init(void);
                        
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);

#endif /* _BSP_TIM_H */