#include "controleModule.h"


/* Elementary functions */
uint8_t readRegister(uint8_t addr) {
        uint8_t cmd = R_REGISTER & addr;
        uint8_t data;
        uint8_t *pCmd = &cmd;
        uint8_t *pData = &data;
        size_t cmdSize = sizeof(cmd);
        size_t dataSize = sizeof(data);
        HAL_SPI_Transmit(&hspi2, pCmd, cmdSize, SPI_TIMEOUT);
        HAL_SPI_Receive(&hspi2, pData, dataSize, SPI_TIMEOUT );
        return data;

}

void writeRegister(uint8_t addr, uint8_t val) {
	uint8_t write = W_REGISTER & addr;
	uint8_t *pWrite = &write;
	size_t writeSize = sizeof(write);
	HAL_SPI_Transmit(&hspi2, pWrite, writeSize, SPI_TIMEOUT);
	HAL_SPI_Transmit(&hspi2, &val, sizeof(val), SPI_TIMEOUT);

}

