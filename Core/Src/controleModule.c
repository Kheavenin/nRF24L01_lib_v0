#include "controleModule.h"


/* Elementary functions */
uint8_t readRegister(uint8_t addr) {
        uint8_t cmd = R_REGISTER & addr;
        uint8_t data;
        uint8_t *pCmd = &cmd;
        uint8_t *pData = &data;
        size_t cmdSize = sizeof(cmd);
        size_t dataSize = sizeof(data);
        if ( HAL_SPI_Transmit(&hspi2, pCmd, cmdSize, SPI_TIMEOUT))
        	HAL_SPI_Receive(&hspi2, pData, dataSize, SPI_TIMEOUT );
        return data;

}

void writeRegister(uint8_t addr, uint8_t val) {
	uint8_t write = W_REGISTER & addr;
	uint8_t *pWrite = &write;
	size_t writeSize = sizeof(write);
	if(HAL_SPI_Transmit(&hspi2, pWrite, writeSize, SPI_TIMEOUT))
		HAL_SPI_Transmit(&hspi2, &val, sizeof(val), SPI_TIMEOUT);

}

uint8_t readBit(uint8_t addr, uint8_t bit) {
	uint8_t reg = readRegister(addr);
	return (reg >> bit) & 0x01;
}

void setBit(uint8_t addr, uint8_t bit) {
	uint8_t tmp = readRegister(addr);
	tmp |= 1 << bit;
	writeRegister(addr, tmp);
}

void resetBit(uint8_t addr, uint8_t bit) {
	uint8_t tmp = readRegister(addr);
	tmp |= 0 << bit;
	writeRegister(addr, tmp);
}

void powerUp() {
	setBit( CONFIG, PWR_UP);
}
void powerDown() {
	resetBit(CONFIG, PWR_UP);

}
