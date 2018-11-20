/*
* Copyright (C) 2018 Adolfo E. García
*
* This file is part of STG-8nn-Scaffold.
*
* STG-8nn-Scaffold is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* STG-8nn-Scaffold is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with STG-8nn-Scaffold.  If not, see <www.gnu.org/licenses/>.
*/

#ifndef BSP_H
#define BSP_H

#include <stdint.h>

#define BSP_TICKS_PER_SEC 10000U /* 1 tick every 0.1 ms (100 us) */
#define BSP_TICKS_PER_MS  (BSP_TICKS_PER_SEC / 10)
#define BSP_TICKS_PER_US  (BSP_TICKS_PER_MS / 10)

/* BSP prototypes */
void BSP_init(void);
void BSP_GPIO_init(void);
void BSP_ledOn(void);
void BSP_ledOff(void);
void BSP_terminate(int16_t result);

#include "stg_850.h" /* Select your STG-8nn */

#endif // BSP_H
