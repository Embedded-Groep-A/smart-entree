#ifndef SERVO_H
#define SERVO_H

#include "stm32f3xx_hal.h"

// Externe declaratie van htim1, zodat deze in servo.c kan worden gebruikt
extern TIM_HandleTypeDef htim1;

// Functie declaratie voor het aansteken van de servo
void servo_control(uint8_t angle);

// Functie declaratie voor het initialiseren van de servo
void servo_init(void);

#endif // SERVO_H
