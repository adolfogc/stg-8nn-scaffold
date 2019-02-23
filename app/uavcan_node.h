#ifndef _UAVCAN_NODE_H
#define _UAVCAN_NODE_H

#include "qpc.h"
#include "canard.h"

/* Active Object */
extern QActive * const AO_UavcanNode;

typedef struct UavcanNodeTag {
    QActive super;
    QTimeEvt timeEvent;
    uint32_t spinCtrl;
} UavcanNode;

UavcanNode* UavcanNode_initAO(void);

CanardInstance* getCanardInstance(void);

#endif /* _UAVCAN_NODE_H */
