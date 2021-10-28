/*
 * winbond.h
 *
 *  Created on: 30 èþë. 2018 ã.
 *      Author: Dmitry
 */

#ifndef WINBOND_H_
#define WINBOND_H_

#include <stdint.h>

#define SPIFLASH_WRITEENABLE      0x06        // write enable
#define SPIFLASH_WRITEDISABLE     0x04        // write disable

#define SPIFLASH_BLOCKERASE_4K    0x20        // erase one 4K block of flash memory
#define SPIFLASH_BLOCKERASE_32K   0x52        // erase one 32K block of flash memory
#define SPIFLASH_BLOCKERASE_64K   0xD8        // erase one 64K block of flash memory
#define SPIFLASH_CHIPERASE        0x60        // chip erase (may take several seconds depending on size)
                                              // but no actual need to wait for completion (instead need to check the status register BUSY bit)
#define SPIFLASH_STATUSREAD       0x05        // read status register
#define SPIFLASH_STATUSWRITE      0x01        // write status register
#define SPIFLASH_ARRAYREAD        0x0B        // read array (fast, need to add 1 dummy byte after 3 address bytes)
#define SPIFLASH_ARRAYREADLOWFREQ 0x03        // read array (low frequency)

#define SPIFLASH_SLEEP            0xB9        // deep power down
#define SPIFLASH_WAKE             0xAB        // deep power wake up
#define SPIFLASH_BYTEPAGEPROGRAM  0x02        // write (1 to 256bytes)
#define SPIFLASH_IDREAD           0x9F        // read JEDEC manufacturer and device ID (2 bytes, specific bytes for each manufacturer and device)

#define SPIFLASH_MACREAD          0x4B        // read unique ID number (MAC)

uint8_t flash_initialize();
void flash_command(uint8_t cmd, uint8_t is_write);
uint8_t flash_read_status();
void flash_read_buffer(uint32_t addr, uint8_t* buf, uint16_t len);
void flash_write_buffer(uint32_t addr, const uint8_t* buf, uint16_t len);
uint8_t flash_is_busy();
void flash_chip_erase();
void flash_block_erase_4K(uint32_t addr);
void flash_block_erase_32K(uint32_t addr);
void flash_block_erase_64K(uint32_t addr);
void flash_sleep();
void flash_wakeup();
void flash_select();
void flash_deselect();

#endif /* WINBOND_H_ */
