#include "sd.h"

void spi_transmit (uint8_t value){
	HAL_SPI_Transmit(&hspi2, &value, 1, TIMEOUT);
}

uint8_t spi_receive (){
	uint8_t ret = 0;
	HAL_SPI_Receive(&hspi2, &ret, 1, TIMEOUT);
	return ret;
}

uint8_t flash_read_status1 (){
	uint8_t ret = 0;

	CS_LOW();

	spi_transmit(CMD_STATUS_1);
	ret = spi_receive();

	CS_HIGH();
	return ret;
}

void flash_wait_write(){

	while ((flash_read_status1() & 0x01) == 0x01){
//		HAL_Delay(1);
	}

}

void flash_send_command(uint8_t cmd){
	CS_LOW();
	spi_transmit(cmd);
	CS_HIGH();
}

void flash_write_enable(){
	flash_send_command(CMD_WRITE_ENABLE);
}

void flash_write_disable(){
	flash_send_command(CMD_WRITE_DISABLE);
}

void flash_full_chip_erase(){
	flash_write_enable();

	flash_send_command(CMD_FULL_ERASE);

	flash_wait_write();
}

void flash_get_jedec_id(uint8_t* buf){
	CS_LOW();
	spi_transmit(CMD_JEDEC_ID);
	for (int i = 0; i < 3; i++){
		buf[i] = spi_receive();
	}
}

void flash_read_block(uint32_t addr, uint8_t* buf, uint16_t size){
//	addr = addr * 0x1000 * 16;
//	size = size * 0x1000 * 16;

	CS_LOW();
	spi_transmit(CMD_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < size; i++){
		buf[i] = spi_receive();
	}

	CS_HIGH();
}

void flash_write_block(uint32_t addr, uint8_t* buf, uint16_t size){
//	addr = addr * 0x1000 * 16;
//	size = size * 0x1000 * 16;

	flash_write_enable();

	CS_LOW();
	spi_transmit(CMD_WRITE);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < size; i ++){
		spi_transmit(buf[i]);

	}
	CS_HIGH();
	flash_wait_write();
}

void flash_erase_block(uint32_t addr){
	//addr = addr * 0x1000 * 16;

	flash_write_enable();

	CS_LOW();

	spi_transmit(CMD_BLOCK_ERASE);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);
	CS_HIGH();
	flash_wait_write();
}

//read 256 byte
void flash_read_page (uint32_t addr, uint8_t* buf){
	CS_LOW();

	spi_transmit(CMD_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < 256; i++){
		buf[i] = spi_receive();
	}

	CS_HIGH();
}

void flash_write_page (uint32_t addr, uint8_t* buf){
	flash_write_enable();

	CS_LOW();

	spi_transmit(CMD_WRITE);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < 256; i++){
		spi_transmit(buf[i]);
	}

	CS_HIGH();

	flash_wait_write();
}

void flash_sector_erase(uint32_t sector_addr){
	flash_write_enable();

	CS_LOW();
	spi_transmit(CMD_SECTOR_ERASE);
	spi_transmit((sector_addr >> 16) & 0xFF);
	spi_transmit((sector_addr >> 8) & 0xFF);
	spi_transmit(sector_addr & 0xFF);
	CS_HIGH();

	flash_wait_write();
}

void flash_read_sector(uint32_t addr, uint8_t* buf){
	CS_LOW();

	spi_transmit(CMD_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < 256 * 16; i++){
		buf[i] = spi_receive();
	}

	CS_HIGH();

}

void flash_write_sector(uint32_t addr, uint8_t* buf){
	flash_write_enable();

	for (int i = 0; i < 16; i++){
		CS_LOW();
		spi_transmit(CMD_WRITE);
		spi_transmit((addr >> 16) & 0xFF);
		spi_transmit((addr >> 8) & 0xFF);
		spi_transmit(addr & 0xFF);

		for (int j = 0; j < 256; j++){
			spi_transmit(buf[j]);
		}

		CS_HIGH();

		flash_wait_write();

		addr += 256;
		buf += 256;
	}
}

void flash_read_sector_2(uint32_t addr, uint8_t* buf){
	CS_LOW();

	spi_transmit(CMD_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	HAL_SPI_Receive(&hspi2, buf, 4096, TIMEOUT);


	CS_HIGH();
}

void flash_fast_read_sector(uint32_t addr, uint8_t* buf){
	CS_LOW();

	spi_transmit(CMD_FAST_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	uint8_t rx,tx = 0;
	HAL_SPI_TransmitReceive(&hspi2, &tx, &rx, 1, TIMEOUT);

	HAL_SPI_Receive(&hspi2, buf, 4096, TIMEOUT);
	CS_HIGH();

}

void flash_read_any_bytes(uint32_t addr, uint8_t* buf, uint32_t size){
	CS_LOW();

	spi_transmit(CMD_READ);
	spi_transmit((addr >> 16) & 0xFF);
	spi_transmit((addr >> 8) & 0xFF);
	spi_transmit(addr & 0xFF);

	for (int i = 0; i < size; i++){
		buf[i] = spi_receive();
	}

	CS_HIGH();
}

