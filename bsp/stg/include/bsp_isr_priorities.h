/*
Copyright (C) 2018-2019 Adolfo E. García

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

#ifndef _BSP_IRS_PRI_H
#define _BSP_IRS_PRI_H

#include "bsp_qpc.h"

/* The QF_AWARE_ISR_CMSIS_PRI constant from the QF port specifies the highest
 * ISR priority that is disabled by the QF framework. The value is suitable
 * for the NVIC_SetPriority() CMSIS function.
 *
 * Only ISRs prioritized at or below the QF_AWARE_ISR_CMSIS_PRI level (i.e.,
 * with the numerical values of priorities equal or higher than
 * QF_AWARE_ISR_CMSIS_PRI) are allowed to call any QF services. These ISRs
 * are "QF-aware".
 */

/* In Cortex-M0, there are 4 priority levels only, given by the upper two bits
 * of the priority byte. No grouping available. All ISRs are KernelAware.
 */
enum KernelAwareISRs {
  BSP_SYSTICK_PRIO = QF_AWARE_ISR_CMSIS_PRI,
  BSP_CAN_PRIO,
  BSP_USART3_PRIO,
  BSP_SVC_PRIO,
  /* ... */
  MAX_KERNEL_AWARE_CMSIS_PRI /* keep always last */
};

#endif /* _BSP_IRS_PRI_H */
