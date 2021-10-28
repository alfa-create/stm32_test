/*
 * platform.h
 *
 *  Created on: 30 èþë. 2018 ã.
 *      Author: Dmitry
 */

#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <stdint.h>

void spi_select();
void spi_deselect();
uint8_t spi_transmit_receive_byte(uint8_t b);

void erase_led_on();
void erase_led_off();
void r_led_on();
void r_led_off();
void w_led_on();
void w_led_off();

#endif /* PLATFORM_H_ */
