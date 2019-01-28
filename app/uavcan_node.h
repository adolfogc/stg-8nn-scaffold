#ifndef _UAVCAN_NODE_H
#define _UAVCAN_NODE_H

#include "qpc.h"

extern QActive * const AO_UavcanNode;

typedef struct UavcanNodeTag {
    QActive super;

    QTimeEvt spinTimeEvent;
    uint16_t pollCtr;
    uint16_t procCtr;
} UavcanNode;

void UavcanNode_ctor(UavcanNode* me);

#endif /* _UAVCAN_NODE_H */