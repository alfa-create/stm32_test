/*
 * winbond.c
 *
 *  Created on: 30 èþë. 2018 ã.
 *      Author: Dmitry
 */
#include <stdlib.h>

#include "winbond.h"
#include "platform.h"

#define MAX_WRITE_BUFFER_SIZE 256

void flash_select() {
	spi_select();
}


void flash_deselect() {
	spi_deselect();
}

uint8_t flash_initialize() {
	flash_deselect();
	flash_wakeup();
    flash_command(SPIFLASH_STATUSWRITE, 1);
    spi_transmit_receive_byte(0);
    flash_deselect();
    return 1;
}

/// read unlimited # of bytes
void flash_read_buffer(uint32_t addr, uint8_t* buf, uint16_t len) {
	r_led_on();
	flash_command(SPIFLASH_ARRAYREAD, 0);
	spi_transmit_receive_byte(addr >> 16);
	spi_transmit_receive_byte(addr >> 8);
	spi_transmit_receive_byte(addr);
	spi_transmit_receive_byte(0);
	for (uint16_t i = 0; i < len; ++i) {
		buf[i] = spi_transmit_receive_byte(0);
	}
	flash_deselect();
	r_led_off();
}

/// Send a command to the flash chip, pass 1 for isWrite when its a write command
void flash_command(uint8_t cmd, uint8_t is_write){
	if (is_write) {
		flash_command(SPIFLASH_WRITEENABLE, 0);
		flash_deselect();
	}
	if (cmd != SPIFLASH_WAKE) while(flash_is_busy());
	flash_select();
	spi_transmit_receive_byte(cmd);
}

/// check if the chip is busy erasing/writing
uint8_t flash_is_busy() {
	return flash_read_status() & 1;
}

/// return the STATUS register
uint8_t flash_read_status() {
	flash_select();
	spi_transmit_receive_byte(SPIFLASH_STATUSREAD);
	uint8_t status = spi_transmit_receive_byte(0);
	flash_deselect();
	return status;
}

/// write multiple bytes to flash memory (up to 64K)
/// WARNING: you can only write to previously erased memory locations (see datasheet)
///          use the block erase commands to first clear memory (write 0xFFs)
/// This version handles both page alignment and data blocks larger than 256 bytes.
///
void flash_write_buffer(uint32_t addr, const uint8_t* buf, uint16_t len) {
	w_led_on();
	uint16_t n;
	uint16_t max_bytes = MAX_WRITE_BUFFER_SIZE-(addr%MAX_WRITE_BUFFER_SIZE);  // force the first set of bytes to stay within the first page
	uint16_t offset = 0;
	while (len>0) {
		n = (len<=max_bytes) ? len : max_bytes;
		flash_command(SPIFLASH_BYTEPAGEPROGRAM, 1);  // Byte/Page Program
		spi_transmit_receive_byte(addr >> 16);
		spi_transmit_receive_byte(addr >> 8);
		spi_transmit_receive_byte(addr);

		for (uint16_t i = 0; i < n; i++) {
			spi_transmit_receive_byte(buf[offset + i]);
		}
		flash_deselect();

		addr+=n;  // adjust the addresses and remaining bytes by what we've just transferred.
		offset +=n;
		len -= n;
		max_bytes = MAX_WRITE_BUFFER_SIZE;   // now we can do up to 256 bytes per loop
	}
	w_led_off();
}

/// erase entire flash memory array
/// may take several seconds depending on size, but is non blocking
/// so you may wait for this to complete using busy() or continue doing
/// other things and later check if the chip is done with busy()
/// note that any command will first wait for chip to become available using busy()
/// so no need to do that twice
void flash_chip_erase() {
	flash_command(SPIFLASH_CHIPERASE, 1);
  	flash_deselect();
}

/// erase a 4Kbyte block
void flash_block_erase_4K(uint32_t addr) {
	erase_led_on();
	flash_command(SPIFLASH_BLOCKERASE_4K, 1); // Block Erase
	spi_transmit_receive_byte(addr >> 16);
	spi_transmit_receive_byte(addr >> 8);
	spi_transmit_receive_byte(addr);
	flash_deselect();
	erase_led_off();
}

/// erase a 32Kbyte block
void flash_block_erase_32K(uint32_t addr) {
	flash_command(SPIFLASH_BLOCKERASE_32K, 1); // Block Erase
	spi_transmit_receive_byte(addr >> 16);
	spi_transmit_receive_byte(addr >> 8);
	spi_transmit_receive_byte(addr);
	flash_deselect();
}

/// erase a 64Kbyte block
void flash_block_erase_64K(uint32_t addr) {
	flash_command(SPIFLASH_BLOCKERASE_64K, 1); // Block Erase
	spi_transmit_receive_byte(addr >> 16);
	spi_transmit_receive_byte(addr >> 8);
	spi_transmit_receive_byte(addr);
	flash_deselect();
}

void flash_sleep() {
	flash_command(SPIFLASH_SLEEP, 0);
	flash_deselect();
}

void flash_wakeup() {
	flash_command(SPIFLASH_WAKE, 0);
	flash_deselect();
}
