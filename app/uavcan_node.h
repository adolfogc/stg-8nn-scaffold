#ifndef _UAVCAN_NODE_H
#define _UAVCAN_NODE_H

#include "qpc.h"

/* Active Object */
extern QActive * const AO_UavcanNode;

typedef struct UavcanNodeTag {
    QActive super;

    QTimeEvt statusUpdateTimeEvent; /* used for periodic node status updates */
    QTimeEvt receiveTimeEvent; /* used for "reception polling" */
    QTimeEvt restartTimeEvent;
} UavcanNode;

UavcanNode* initUavcanNode(void);

#endif /* _UAVCAN_NODE_H */