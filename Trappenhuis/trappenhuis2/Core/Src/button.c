#include "button.h"

#define BUTTON_PIN GPIO_PIN_10  // Pas dit aan op basis van je GPIO-pin (PA10 in plaats van PA1)
#define BUTTON_PORT GPIOB       // Port voor PA10

void button_init(void) {
    // Configureer de knop als input
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = BUTTON_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(BUTTON_PORT, &GPIO_InitStruct);
}

uint8_t button_is_pressed(void) {
    // Retourneer of de knop ingedrukt is (je kunt een UART print toevoegen om te debuggen)
	uint8_t waardeButton = HAL_GPIO_ReadPin(BUTTON_PORT, BUTTON_PIN)==GPIO_PIN_SET ? 1:0;
     	 return waardeButton;


}
