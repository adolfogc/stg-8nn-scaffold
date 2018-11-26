#ifndef _BSP_USART_H
#define _BSP_USART_H

#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart1;
extern IRDA_HandleTypeDef hirda3;

void BSP_USART1_UART_Init(void);
void BSP_USART3_IRDA_Init(void);

#endif /* _BSP_USART_H */