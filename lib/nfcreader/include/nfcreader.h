#ifndef RC522_H
#define RC522_H

#include "stm32f1xx_hal.h"

void rc522_init(void);
uint8_t rc522_is_card_present(void);
uint8_t rc522_read_uid(uint8_t *uid_out);

#endif
