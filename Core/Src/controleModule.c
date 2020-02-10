#include "controleModule.h"

/**
 * #if and #endif preproccessor directive used to test code variants.
 */

/**
 * @Brief	Read register function
 * @Param	addr - address of register to read.
 * @Ratval	return read register content.
 */
uint8_t readRegister(uint8_t addr) {
	uint8_t cmd = R_REGISTER | addr;
	uint8_t reg;
	uint8_t *pCmd = &cmd;
	uint8_t *pReg = &reg;
	size_t cmdSize = sizeof(cmd);
	size_t regSize = sizeof(reg);
	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
	csnLow();
#if 0
	HAL_StatusTypeDef statusSend;
	HAL_StatusTypeDef statusRead;
	while (!statusRead) {
		statusSend = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
		if (statusSend) {
			statusRead = HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT);
		}
	}
#endif
#if 0
	HAL_StatusTypeDef statusRead;
	HAL_StatusTypeDef statusCmd;
	statusRead = HAL_SPI_TransmitReceive(&hspi1, pCmd, pReg, cmdSize,
	SPI_TIMEOUT);
#endif
#if 0
	if (HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT))
		HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT);
#endif
#if 1
	HAL_StatusTypeDef statusRead;
	HAL_StatusTypeDef statusCmd;
	statusCmd = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
	HAL_Delay(1);
	statusRead = HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT);
#endif
	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_SET);
	csnHigh();
	return reg;
}

/**
 * @Brief	Write register funtion.
 * @Param	addr - address of register to write
 * @Param	val - value to write into register.
 */
void writeRegister(uint8_t addr, uint8_t val) {
	uint8_t cmd = W_REGISTER | addr;
	uint8_t *pCmd = &cmd;
	size_t cmdSize = sizeof(cmd);
	size_t valSize = sizeof(val);

	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
	csnLow();
#if 1
	HAL_StatusTypeDef statusSend;
	HAL_StatusTypeDef statusRead;
	statusSend = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
	statusRead = HAL_SPI_Transmit(&hspi1, &val, valSize, SPI_TIMEOUT);
#endif
#if 0
	HAL_StatusTypeDef statusSend;
	HAL_StatusTypeDef statusRead;
	while (!statusRead) {
		statusSend = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
		if (statusSend) {
			statusRead = HAL_SPI_Transmit(&hspi1, &val, valSize, SPI_TIMEOUT);
		}
	}
#endif
#if 0
	if (HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT)) {
		HAL_SPI_Transmit(&hspi1, &val, valSize, SPI_TIMEOUT);
	}
	csnHigh();
	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_SET);
#endif
}

/**
 * @Brief	Get Status Register.
 * @Retval	Return value of Status Register
 */
uint8_t getStatus() {
	uint8_t cmd = NOP;
	uint8_t *pCmd = &cmd;
	size_t cmdSize = sizeof(cmd);

	uint8_t reg = 0;
	uint8_t *pReg = &reg;
	size_t regSize = sizeof(reg);

	csnLow();
	if (HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT))
		HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT);
	csnHigh();

	return reg;
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

/**
 * @Brief	Set low level on CSN line
 */
void csnLow() {
	HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
}

/**
 * @Brief	Set high level on CSN line
 */
void csnHigh() {
	HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_SET);
}

/**
 * @Brief	Set low level on CE line
 */
void ceLow() {
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_RESET);
}

/**
 * @Brief	Set high level on CE line
 */
void ceHigh() {
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
}
