/*
Copyright (C) 2018 Adolfo E. García

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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "bsp.h"
#include "uavcan/protocol/HardwareVersion.h"

static time_t g_startTime;

void BSP_init(void)
{
  g_startTime = time(NULL);
}

void BSP_restart(void)
{
    fputs("RESTARTING...\n", stdout);
    QF_stop();
}

void BSP_Led_off(void)
{
    fputs("LED is OFF\n", stdout);
}

void BSP_Led_on(void)
{
    fputs("LED is ON\n", stdout);
}

uint32_t BSP_upTimeSeconds(void)
{
    const double upTime_ = difftime(time(NULL), g_startTime);
    return (uint32_t) upTime_;
}

uint32_t BSP_getPseudoRandom(void)
{
  static bool uninitialized = true;
  if(uninitialized) {
    srand(time(NULL));
    uninitialized = false;
  }

  return (uint32_t)rand();
}

void BSP_readUniqueID(uint8_t* outUid)
{
    for (uint8_t i = 0; i < UAVCAN_PROTOCOL_HARDWAREVERSION_UNIQUE_ID_LENGTH; ++i) {
        outUid[i] = i;
    }
}
