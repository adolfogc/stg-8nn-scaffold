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

#ifndef _SCAFFOLD_APP_H
#define _SCAFFOLD_APP_H

#include "bsp.h"
#include "led.h"
#include "uavcan_node.h"

#include "app_signals.h"

#ifdef APP_ENABLE_CUSTOM_SIGNALS
#include "app_custom_signals.h"
#else
enum AppSignals {
  APP_MAX_SIG = APP_BASE_MAX_SIG /* last app signal */
};
#endif

#ifdef APP_ENABLE_CUSTOM_BUILD_CONFIG
#include "app_config.h"
#endif

/* Stack size in bytes */
#ifndef APP_STACK_SIZE
#define APP_STACK_SIZE 0x1000
#endif

/* Heap size in bytes */
#ifndef APP_HEAP_SIZE
#define APP_HEAP_SIZE 0x200
#endif

#ifndef APP_SW_VERSION_MAJOR
#define APP_SW_VERSION_MAJOR 0
#endif

#ifndef APP_SW_VERSION_MINOR
#define APP_SW_VERSION_MINOR 1
#endif

#ifndef APP_HW_VERSION_MAJOR
#define APP_HW_VERSION_MAJOR 0
#endif

#ifndef APP_HW_VERSION_MINOR
#define APP_HW_VERSION_MINOR 1
#endif

/*
  Possible bit rates:

  1000 kbit/s = 1000000U
   500 kbit/s =  500000U
   250 kbit/s =  250000U
   125 kbit/s =  125000U

  See: https://uavcan.org/Specification/8._Hardware_design_recommendations
*/
#ifndef APP_CAN_BITRATE
#define APP_CAN_BITRATE 125000U
#endif

#ifndef APP_UAVCAN_NODE_NAME_DATA
#define APP_UAVCAN_NODE_NAME_DATA "local.stg8nn.scaffold"
#define APP_UAVCAN_NODE_NAME_LEN sizeof(APP_UAVCAN_NODE_NAME_DATA) - 1U
#endif

#ifndef APP_UAVCAN_DEFAULT_NODE_ID
#define APP_UAVCAN_DEFAULT_NODE_ID 45U
#endif

#ifndef APP_UAVCAN_NODE_EVT_QUEUE_SIZE
#define APP_UAVCAN_NODE_EVT_QUEUE_SIZE 20
#endif

#ifndef APP_LED_EVT_QUEUE_SIZE
#define APP_LED_EVT_QUEUE_SIZE 10
#endif

#ifndef APP_CANARD_MEMORY_POOL_SIZE
#define APP_CANARD_MEMORY_POOL_SIZE 4096
#endif

#endif /* _SCAFFOLD_APP_H */
