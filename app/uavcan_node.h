#ifndef _UAVCAN_NODE_H
#define _UAVCAN_NODE_H

#include "qpc.h"

/* Active Object */
extern QActive * const AO_UavcanNode;

typedef struct UavcanNodeTag {
    QActive super;
    QTimeEvt timeEvent;
    uint32_t spinCtrl;
} UavcanNode;

UavcanNode* initUavcanNode(void);

#endif /* _UAVCAN_NODE_H */
