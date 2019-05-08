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

#include <stdint.h>
#include <stdbool.h>
#include "canard.h"
#include "bsp_qpc.h"
#include "bsp_specific.h"

void BSP_init(void);
void BSP_restart(void) __attribute__((noreturn));

void BSP_Led_off(void);
void BSP_Led_on(void);

typedef enum {
    BSP_CAN_RXTX_TIMEOUT,
    BSP_CAN_RXTX_SUCCESS,
    BSP_CAN_RXTX_ERROR
} BSP_CAN_RxTxResult;

typedef enum {
    BSP_CAN_FILTER_ACCEPT_ALL = 1,
    BSP_CAN_FILTER_ACCEPT_ANONYMOUS,
    BSP_CAN_FILTER_ACCEPT_ALL_FROM_DEBUGGER,
    BSP_CAN_FILTER_USE_ADDITIONAL_RULES
} BSP_CAN_FilterOptions;

typedef struct {
    uint32_t id;
    uint32_t mask;
} BSP_CAN_FilterRule;

typedef struct {
    BSP_CAN_FilterOptions options;
    BSP_CAN_FilterRule * const rules;
    uint_fast8_t const MaxRuleNumber;
    uint_fast8_t nRules;
} BSP_CAN_FilterConfig;

BSP_CAN_FilterConfig * BSP_CAN_getFilterConfig(void);

/* Accepts all frames with the given source nodes and regardless of message ids */
void BSP_CAN_newMessageFilter1(BSP_CAN_FilterRule* rule, const uint32_t srcNodes);

/* Accepts all frames with the given message ids and regardless of source node */
void BSP_CAN_newMessageFilter2(BSP_CAN_FilterRule* rule, const uint32_t messageIds);

/* Accepts all frames with the given source nodes and message ids */
void BSP_CAN_newMessageFilter3(BSP_CAN_FilterRule* rule, const uint32_t srcNodes, const uint32_t messageIds);

/* Accepts all frames with the given source and destination nodes and regardless of service id */
void BSP_CAN_newServiceFilter1(BSP_CAN_FilterRule* rule, const uint32_t srcNodes, const uint32_t destNodes);

/* Accepts all frames with the given service ids and regardless of source/destination nodes */
void BSP_CAN_newServiceFilter2(BSP_CAN_FilterRule* rule, const uint32_t serviceIds);

/* Accepts all frames with the given source and destination nodes as well as the given service ids */
void BSP_CAN_newServiceFilter3(BSP_CAN_FilterRule* rule, const uint32_t srcNodes, const uint32_t destNodes, const uint32_t serviceIds);

/* Accepts all frames with the given destination nodes, regardless of source node or service id */
void BSP_CAN_newServiceFilter4(BSP_CAN_FilterRule* rule, const uint32_t destNodes);

bool BSP_CAN_init(BSP_CAN_FilterConfig * const filterConfig);

BSP_CAN_RxTxResult BSP_CAN_transmitOnce(const CanardCANFrame* frame);
BSP_CAN_RxTxResult BSP_CAN_receiveOnce(CanardCANFrame* frame);

uint32_t BSP_upTimeSeconds(void);
void BSP_readUniqueID(uint8_t* outUid);
uint32_t BSP_getPseudoRandom(void);

void BSP_Ticker0_initAO(void);
void BSP_Ticker0_startAO(uint8_t priority);
QTicker* BSP_Ticker0_getAO(void);

#endif /* _BSP_H */
