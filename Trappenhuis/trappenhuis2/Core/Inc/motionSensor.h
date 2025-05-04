// motion_sensor.h
#ifndef MOTIONSENSOR_H
#define MOTIONSENSOR_H

#include "stm32f3xx_hal.h"

// Functies voor de bewegingssensor
void motion_sensor_init(void);
uint8_t motionSensor_detect(void);

#endif // MOTION_SENSOR_H
