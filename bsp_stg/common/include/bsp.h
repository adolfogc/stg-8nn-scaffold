/*
Copyright (C) 2019 Adolfo E. García

This file is part of STG-8nn-Scaffold.

STG-8nn-Scaffold is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

STG-8nn-Scaffold is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _BSP_H
#define _BSP_H

#include "stm32f0xx_hal.h"

#include "bsp_irs_pri.h"
#include "bsp_pinout.h"
#include "bsp_qpc.h"

#include "adc.h"
#include "can.h"
#include "gpio.h"
#include "i2c.h"
#include "iwdg.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"

#define BSP_TICKS_PER_SEC 1000U

void BSP_Init(void);
void BSP_ledOff(void);
void BSP_ledOn(void);

#endif /* _BSP_H */