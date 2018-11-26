#include "bsp/iwdg.h"
#include "bsp/pinout.h"
#include "bsp/bsp_qpc.h"

Q_DEFINE_THIS_FILE

IWDG_HandleTypeDef hiwdg;

void MX_IWDG_Init(void)
{
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_4;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  Q_ENSURE(HAL_IWDG_Init(&hiwdg) == HAL_OK);
}