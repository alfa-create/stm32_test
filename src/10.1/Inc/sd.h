#pragma once
#include "spi.h"

extern SPI_HandleTypeDef hspi2;

#define CS_LOW() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define CS_HIGH() HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
#define TIMEOUT 0x1000

#define CMD_WRITE_ENABLE 0x06
#define CMD_WRITE_DISABLE 0x04
#define CMD_READ 0x03
#define CMD_WRITE 0x02
#define CMD_STATUS_1 0x05
#define CMD_FULL_ERASE 0xC7
#define CMD_JEDEC_ID 0x9F
#define CMD_BLOCK_ERASE 0xD8
#define CMD_DEVICE_ID 0x90
#define CMD_SECTOR_ERASE 0x20
#define CMD_FAST_READ 0x0B

void spi_transmit (uint8_t value);
uint8_t spi_receive ();
uint8_t flash_read_status1 ();
void flash_wait_write();
void flash_send_command(uint8_t cmd);
void flash_write_enable();
void flash_write_disable();
void flash_chip_erase();
void flash_get_jedec_id(uint8_t* buf);

void flash_read_block(uint32_t addr, uint8_t* buf, uint16_t size);
void flash_write_block(uint32_t addr, uint8_t* buf, uint16_t size);
void flash_erase_block(uint32_t addr);

void flash_write_page (uint32_t addr, uint8_t* buf);
void flash_read_page (uint32_t addr, uint8_t* buf);
void flash_sector_erase(uint32_t sector_addr);

void flash_read_sector(uint32_t addr, uint8_t* buf);
void flash_write_sector(uint32_t addr, uint8_t* buf);

void flash_read_sector_2(uint32_t addr, uint8_t* buf);
void flash_fast_read_sector(uint32_t addr, uint8_t * buf);

void flash_read_any_bytes(uint32_t addr, uint8_t* buf, uint32_t size);
