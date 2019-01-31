#ifndef _UAVCAN_NODE_H
#define _UAVCAN_NODE_H

#include "qpc.h"

/* Active Object */
extern QActive * const AO_UavcanNode;

typedef struct UavcanNodeTag {
    QActive super;

    QTimeEvt spinTimeEvent;
} UavcanNode;

void UavcanNode_ctor(UavcanNode* me);

/* Public functions */
void APP_Canard_initInstance(void);

#endif /* _UAVCAN_NODE_H */