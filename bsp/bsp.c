/*
* Copyright (C) 2018 Adolfo E. García
*
* This file is part of STG-8nn-Scaffold.
*
* STG-8nn-Scaffold is free software: you can redistribute it and/or modify
* it under the terms of the GNU Affero General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* STG-8nn-Scaffold is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Affero General Public License for more details.
*
* You should have received a copy of the GNU Affero General Public License
* along with STG-8nn-Scaffold.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include "bsp.h"

// LED in STG-8nn
#define LED_PIN GPIO8
#define LED_PORT GPIOA
#define LED_RCC RCC_GPIOA

void BSP_ledOn(void) {
  gpio_set(LED_PORT, LED_PIN);
}

void BSP_ledOff(void) {
  gpio_clear(LED_PORT, LED_PIN);
}

void BSP_init(void) {
  /* Clock the LED_PORT */
  rcc_periph_clock_enable(LED_RCC);
  /* Configure the pin as an output */
  gpio_mode_setup(LED_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_PIN);
  gpio_set_output_options(LED_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, LED_PIN);
  gpio_clear(LED_PORT, LED_PIN);
}

void BSP_terminate(int16_t result) {
    (void)result;
}
