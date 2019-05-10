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

#include "stm32f0xx_hal.h"
#include "drivers/stm32/canard_stm32.h"
#include "app.h"

Q_DEFINE_THIS_FILE

/*
    -- UAVCAN IDENTIFIER MASKS --

    UAVCAN only uses the CAN 2.0B frame format (29-bit identifiers).

    1. MESSAGE FRAME
    ------------------------------------------------------------------------------------------------- 
    |    Priority    |                Message Type ID                  | SNM |    Source Node ID    |    
    | 28 27 26 25 24 | 23 22 21 20 19 18 17 16 15 14 13 12 11 10 09 08 |  07 | 06 05 04 03 02 01 00 |
    ------------------------------------------------------------------------------------------------- 
    SNM (service not message) = 0

    2. ANONYMOUS MESSAGE FRAME
    --------------------------------------------------------------------------------------------------- 
    |    Priority    |           Discriminator                   |  LBS  | SNM |    Source Node ID    |    
    | 28 27 26 25 24 | 23 22 21 20 19 18 17 16 15 14 13 12 11 10 | 09 08 |  07 | 06 05 04 03 02 01 00 |
    --------------------------------------------------------------------------------------------------- 
    SNM = 0
    Discriminator = <random data>
    LBS (lower bits of message type ID)

    3. SERVICE FRAME
    ------------------------------------------------------------------------------------------------------ 
    |    Priority    |     Service Type ID     | RNR |   Dest. Node ID      | SNM |    Source Node ID    |    
    | 28 27 26 25 24 | 23 22 21 20 19 18 17 16 |  15 | 14 13 12 11 10 09 08 |  07 | 06 05 04 03 02 01 00 |
    ------------------------------------------------------------------------------------------------------
    SNM (service not message) = 0
    RNR (request not response)

    Source: https://uavcan.org/Specification/4._CAN_bus_transport_layer
*/

/* The filter management code, i.e., function BSP_CAN_setupFilters/1 and the following constants, is inspired in Libuavcan's:
   See:
     - https://github.com/UAVCAN/libuavcan/blob/dfcdf22eda16ff06847976fd6c7f40671fc92eb5/libuavcan/include/uavcan/transport/can_acceptance_filter_configurator.hpp
     - https://github.com/UAVCAN/libuavcan/blob/ee272fd48b614da92d8532527d61ebca55c561d2/libuavcan/src/transport/uc_can_acceptance_filter_configurator.cpp
*/

static uint32_t const Filter1MsgMask = 0xFFU;     /* 00000 00000000 00000000 11111111 */
static uint32_t const Filter2MsgMask = 0xFFFF80U; /* 00000 11111111 11111111 10000000 */
static uint32_t const Filter3MsgMask = 0xFFFFFFU; /* 00000 11111111 11111111 11111111 */

static uint32_t const BaseFilterSrvId = 0x80U;    /* 00000 00000000 00000000 10000000 */

static uint32_t const Filter1SrvMask = 0x7FFFU;   /* 00000 00000000 01111111 11111111 */
static uint32_t const Filter2SrvMask = 0xFF0080U; /* 00000 11111111 00000000 10000000 */
static uint32_t const Filter3SrvMask = 0xFF7FFFU; /* 00000 11111111 01111111 11111111 */
static uint32_t const Filter4SrvMask = 0x7F80U;   /* 00000 00000000 01111111 10000000 */

static uint32_t const DefaultAnonMsgMask = 0xFFU; /* 00000 00000000 00000000 11111111 */
static uint32_t const DefaultAnonMsgID = 0x0U;    /* 00000 00000000 00000000 00000000 */

/* -- Local state variables -- */

static BSP_CAN_FilterRule l_additionalFilterRules[CANARD_STM32_NUM_ACCEPTANCE_FILTERS];
static BSP_CAN_FilterConfig l_filterConfig = {
    .options = BSP_CAN_FILTER_ACCEPT_ALL_FROM_DEBUGGER,
    .rules = &l_additionalFilterRules[0U],
    .MaxRuleNumber = CANARD_STM32_NUM_ACCEPTANCE_FILTERS,
    .nRules = 0U
};

/* -- Local prototypes -- */

static bool BSP_CAN_setupFilters(BSP_CAN_FilterConfig * const filterConfig);

/* -- Implementation -- */

BSP_CAN_FilterConfig * BSP_CAN_getFilterConfig(void)
{
    return &l_filterConfig;
}

/* Accepts all frames with the given source node and regardless of message id */
void BSP_CAN_newMessageFilter1(BSP_CAN_FilterRule* rule, const uint32_t srcNode)
{
    rule->id = 0U;
    rule->id |= (srcNode & 0x7FU);
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter1MsgMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given message id and regardless of source node */
void BSP_CAN_newMessageFilter2(BSP_CAN_FilterRule* rule, const uint32_t messageId)
{
    rule->id = 0U;
    rule->id |= (messageId & 0xFFFFU) << 8U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter2MsgMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given source node and message id */
void BSP_CAN_newMessageFilter3(BSP_CAN_FilterRule* rule, const uint32_t srcNode, const uint32_t messageId)
{
    rule->id = 0U;
    rule->id |= (srcNode & 0x7FU);
    rule->id |= (messageId & 0xFFFFU) << 8U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter3MsgMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given source and destination nodes and regardless of service id */
void BSP_CAN_newServiceFilter1(BSP_CAN_FilterRule* rule, const uint32_t srcNode, const uint32_t destNode)
{
    rule->id = BaseFilterSrvId;
    rule->id |= (srcNode & 0x7FU);
    rule->id |= (destNode & 0x7FU) << 8U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter1SrvMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given service id and regardless of source/destination node */
void BSP_CAN_newServiceFilter2(BSP_CAN_FilterRule* rule, const uint32_t serviceId)
{
    rule->id = BaseFilterSrvId;
    rule->id |= (serviceId & 0xFFU) << 16U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter2SrvMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given source and destination node as well as the given service id */
void BSP_CAN_newServiceFilter3(BSP_CAN_FilterRule* rule, const uint32_t srcNode, const uint32_t destNode, const uint32_t serviceId)
{
    rule->id = BaseFilterSrvId;
    rule->id |= (srcNode & 0x7FU);
    rule->id |= (destNode & 0x7FU) << 8U;
    rule->id |= (serviceId & 0xFFU) << 16U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter3SrvMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
}

/* Accepts all frames with the given destination node, regardless of source node or service id */
void BSP_CAN_newServiceFilter4(BSP_CAN_FilterRule* rule, const uint32_t destNode) 
{
    rule->id = BaseFilterSrvId;
    rule->id |= (destNode & 0x7FU) << 8U;
    rule->id |= CANARD_CAN_FRAME_EFF;

    rule->mask = Filter4SrvMask;
    rule->mask |= CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR; 
}

bool BSP_CAN_init(BSP_CAN_FilterConfig * const filterConfig)
{
    CanardSTM32CANTimings timings;
    bool success = false;

    /* Change CAN transceiver from silent to active mode. */
    LL_GPIO_ResetOutputPin(CAN_GPIO_Port, CAN_S_Pin);

    /* Initialize the STM32 CAN driver */
    Q_ENSURE(canardSTM32ComputeCANTimings(HAL_RCC_GetPCLK1Freq(), APP_CAN_BITRATE, &timings) == 0U);
    if(canardSTM32Init(&timings, CanardSTM32IfaceModeNormal) == 0U) {
        success = (filterConfig != NULL) ? BSP_CAN_setupFilters(filterConfig) : true;
    }

    return success;
}

BSP_CAN_RxTxResult BSP_CAN_transmitOnce(const CanardCANFrame* frame)
{
    const int16_t txRes = canardSTM32Transmit(frame);
    BSP_CAN_RxTxResult result = BSP_CAN_RXTX_ERROR;
    if (txRes > 0) {
        result = BSP_CAN_RXTX_SUCCESS;
    } else {
        if (txRes == 0) {
            result = BSP_CAN_RXTX_TIMEOUT;
        }
    }
    return result;
}

BSP_CAN_RxTxResult BSP_CAN_receiveOnce(CanardCANFrame* frame)
{
    const int16_t rxRes = canardSTM32Receive(frame);
    BSP_CAN_RxTxResult result = BSP_CAN_RXTX_ERROR;
    if (rxRes > 0) {
        result = BSP_CAN_RXTX_SUCCESS;
    } else {
        if (rxRes == 0) {
            /* No frames to read */
            result = BSP_CAN_RXTX_TIMEOUT;
        }
    }
    return result;
}

static bool BSP_CAN_setupFilters(BSP_CAN_FilterConfig * const filterConfig)
{
    static CanardSTM32AcceptanceFilterConfiguration acceptanceRules[CANARD_STM32_NUM_ACCEPTANCE_FILTERS] = {{0U, 0U}};

    uint_fast8_t next = 0U;

    Q_ENSURE(filterConfig != NULL);

    if(filterConfig->options == BSP_CAN_FILTER_NO_OPTIONS) {
        return false;
    }

    if(filterConfig->options & BSP_CAN_FILTER_ACCEPT_ALL) {
        Q_ENSURE(next < CANARD_STM32_NUM_ACCEPTANCE_FILTERS);
        ++next;
        return canardSTM32ConfigureAcceptanceFilters(acceptanceRules, (uint8_t)next) == 0U;
    }

    if(filterConfig->options & BSP_CAN_FILTER_ACCEPT_ANONYMOUS) {
        Q_ENSURE(next < CANARD_STM32_NUM_ACCEPTANCE_FILTERS);
        acceptanceRules[next].id = DefaultAnonMsgID | CANARD_CAN_FRAME_EFF;
        acceptanceRules[next].mask = DefaultAnonMsgMask | CANARD_CAN_FRAME_EFF | CANARD_CAN_FRAME_RTR | CANARD_CAN_FRAME_ERR;
        ++next;
    }

    if(filterConfig->options & BSP_CAN_FILTER_ACCEPT_ALL_FROM_DEBUGGER) {
        Q_ENSURE(next < CANARD_STM32_NUM_ACCEPTANCE_FILTERS);
        /* Allow all service requests/responses sent by debugging tool with node id of 127 */
        BSP_CAN_newServiceFilter1((BSP_CAN_FilterRule*)&acceptanceRules[next], 127U, APP_UAVCAN_DEFAULT_NODE_ID);
        ++next;

        Q_ENSURE(next < CANARD_STM32_NUM_ACCEPTANCE_FILTERS);
        /* Allow all service requests/responses sent by debugging tool with node id of 126 */
        BSP_CAN_newServiceFilter1((BSP_CAN_FilterRule*)&acceptanceRules[next], 126U, APP_UAVCAN_DEFAULT_NODE_ID);
        ++next;
    }

    if((filterConfig->options & BSP_CAN_FILTER_USE_ADDITIONAL_RULES) &&
       (filterConfig->rules != NULL) &&
       (0 < filterConfig->nRules) &&
       (filterConfig->nRules <= CANARD_STM32_NUM_ACCEPTANCE_FILTERS) &&
       (next < CANARD_STM32_NUM_ACCEPTANCE_FILTERS)) {

        const uint_fast8_t nAvailableEntries = CANARD_STM32_NUM_ACCEPTANCE_FILTERS - next;
        for(uint_fast8_t i = 0U; i < filterConfig->nRules && i < nAvailableEntries; ++i) {
            acceptanceRules[next].id = filterConfig->rules[i].id;
            acceptanceRules[next].mask = filterConfig->rules[i].mask;
            ++next;
        }
    }

    Q_ENSURE(next <= CANARD_STM32_NUM_ACCEPTANCE_FILTERS);
    return canardSTM32ConfigureAcceptanceFilters(acceptanceRules, (uint8_t)next) == 0U;
}