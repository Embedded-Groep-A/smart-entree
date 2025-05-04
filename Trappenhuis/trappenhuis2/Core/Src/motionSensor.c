
#include "motionSensor.h"

#define MOTION_SENSOR_PIN GPIO_PIN_10  // Pas dit aan op basis van je GPIO-pin
#define MOTION_SENSOR_PORT GPIOA

void motion_sensor_init(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = MOTION_SENSOR_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(MOTION_SENSOR_PORT, &GPIO_InitStruct);
}


uint8_t motionSensor_detect(void) {
    // Retourneer of er beweging gedetecteerd is (bijv. logische 1 als er beweging is)

    		uint8_t waardeMotion = HAL_GPIO_ReadPin(MOTION_SENSOR_PORT, MOTION_SENSOR_PIN)==GPIO_PIN_SET ? 1:0;;
    		return waardeMotion;
}
