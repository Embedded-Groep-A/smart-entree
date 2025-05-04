// button.h
#ifndef BUTTON_H
#define BUTTON_H

#include "stm32f3xx_hal.h"

// Functies voor de knop

void button_init(void);
uint8_t button_is_pressed(void);

#endif // BUTTON_H
