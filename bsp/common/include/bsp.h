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

#include "bsp_qpc.h"
#include "bsp_specific.h"
#include "canard.h"
#include <stdbool.h>
#include <stdint.h>

void BSP_init(void);
void BSP_restart(void) __attribute__((noreturn));

void BSP_Led_off(void);
void BSP_Led_on(void);

typedef enum { BSP_CAN_RXTX_TIMEOUT, BSP_CAN_RXTX_SUCCESS, BSP_CAN_RXTX_ERROR } BSP_CAN_RxTxResult;

typedef enum {
  BSP_CAN_FILTER_NO_OPTIONS = 0,
  BSP_CAN_FILTER_ACCEPT_ALL = 1,
  BSP_CAN_FILTER_ACCEPT_ANONYMOUS = 2,
  BSP_CAN_FILTER_ACCEPT_ALL_FROM_DEBUGGER = 4,
  BSP_CAN_FILTER_USE_ADDITIONAL_RULES = 8
} BSP_CAN_FilterOptions;

typedef struct {
  uint32_t id;
  uint32_t mask;
} BSP_CAN_FilterRule;

typedef struct {
  BSP_CAN_FilterOptions options;
  BSP_CAN_FilterRule *const rules;
  uint_fast8_t const MaxRuleNumber;
  uint_fast8_t nRules;
} BSP_CAN_FilterConfig;

BSP_CAN_FilterConfig *BSP_CAN_getFilterConfig(void);

/* Accepts all frames with the given source node and regardless of message id */
void BSP_CAN_newMessageFilter1(BSP_CAN_FilterRule *rule, const uint32_t srcNode);

/* Accepts all frames with the given message id and regardless of source node */
void BSP_CAN_newMessageFilter2(BSP_CAN_FilterRule *rule, const uint32_t messageId);

/* Accepts all frames with the given source node and message id */
void BSP_CAN_newMessageFilter3(BSP_CAN_FilterRule *rule, const uint32_t srcNode, const uint32_t messageId);

/* Accepts all frames with the given source and destination nodes and regardless of service id */
void BSP_CAN_newServiceFilter1(BSP_CAN_FilterRule *rule, const uint32_t srcNode, const uint32_t destNode);

/* Accepts all frames with the given service id and regardless of source/destination node */
void BSP_CAN_newServiceFilter2(BSP_CAN_FilterRule *rule, const uint32_t serviceId);

/* Accepts all frames with the given source and destination node as well as the given service id */
void BSP_CAN_newServiceFilter3(BSP_CAN_FilterRule *rule, const uint32_t srcNode, const uint32_t destNode,
                               const uint32_t serviceId);

/* Accepts all frames with the given destination node, regardless of source node or service id */
void BSP_CAN_newServiceFilter4(BSP_CAN_FilterRule *rule, const uint32_t destNode);

bool BSP_CAN_init(BSP_CAN_FilterConfig *const filterConfig);

BSP_CAN_RxTxResult BSP_CAN_transmitOnce(const CanardCANFrame *frame);
BSP_CAN_RxTxResult BSP_CAN_receiveOnce(CanardCANFrame *frame);

uint32_t BSP_upTimeSeconds(void);
void BSP_readUniqueID(uint8_t *outUid);

void BSP_Ticker0_initAO(void);
void BSP_Ticker0_startAO(uint8_t priority);
QTicker *BSP_Ticker0_getAO(void);

#endif /* _BSP_H */
