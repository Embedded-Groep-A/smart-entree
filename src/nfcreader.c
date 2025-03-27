#include "stm32l4xx_hal.h"       // Algemene HAL-functionaliteit
#include "stm32l4xx_hal_gpio.h"  // GPIO functies
#include "stm32l4xx_hal_spi.h"   // SPI functies
#include "stm32l4xx_hal_uart.h"  // UART functies
#include "stm32l4xx_hal_rcc.h"   // Clock config (optioneel)
#include "string.h"
#include "stdio.h"



extern SPI_HandleTypeDef hspi1;

#define RFID_CS_PORT GPIOA
#define RFID_CS_PIN  GPIO_PIN_4

#define RFID_RST_PORT GPIOA
#define RFID_RST_PIN  GPIO_PIN_3

#define LED_PORT GPIOA
#define LED_PIN  GPIO_PIN_2

void debug_print(char *msg) {
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
}

void System_Init() {
    HAL_GPIO_WritePin(RFID_CS_PORT, RFID_CS_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(RFID_RST_PORT, RFID_RST_PIN, GPIO_PIN_SET);
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_SPI1_Init();
    MX_USART2_UART_Init(); // Voor debug via UART

    System_Init();
    rc522_init(); // Init MFRC522

    debug_print("Scan je RFID kaart...\r\n");

    while (1)
    {
        uint8_t uid[5]; // UID buffer
        if (rc522_is_card_present()) {
            if (rc522_read_uid(uid)) {
                HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

                char msg[64];
                sprintf(msg, "USER ID tag: %02X %02X %02X %02X %02X\r\n", uid[0], uid[1], uid[2], uid[3], uid[4]);
                debug_print(msg);

                HAL_Delay(1000);
                HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
            }
        }
        HAL_Delay(200);
    }
}
