#ifndef BSP_H
#define BSP_H

#include <stdint.h>

#define BSP_TICKS_PER_SEC 100U // 1 tick every 10 ms

void BSP_init(void);
void BSP_terminate(int16_t result);

void BSP_ledOn(void);
void BSP_ledOff(void);

#endif // BSP_H
