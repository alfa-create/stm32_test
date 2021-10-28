/*
 * platform.c
 *
 *  Created on: 30 èþë. 2018 ã.
 *      Author: Dmitry
 */

#include "platform.h"
#include "stm32f1xx_hal.h"
#include "gpio.h"
#include "spi.h"

void spi_select() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void spi_deselect() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

uint8_t spi_transmit_receive_byte(uint8_t b) {
	uint8_t data = 0;
	HAL_SPI_TransmitReceive(&hspi2, &b, &data, 1, 0x1000);
	while (HAL_SPI_GetState(&hspi2) != HAL_SPI_STATE_READY);
	return data;
}

void erase_led_on() {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
}

 void erase_led_off() {
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
}

void r_led_on() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
}

void r_led_off() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
}

void w_led_on() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}

void w_led_off() {
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
}
