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
        uint8_t cmd = R_REGISTER & addr;
        uint8_t reg;
        uint8_t *pCmd = &cmd;
        uint8_t *pReg = &reg;
        size_t cmdSize = sizeof(cmd);
        size_t regSize = sizeof(reg);
        //HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
        csnLow();
#if 0
        HAL_StatusTypeDef status;
        statusRead = HAL_SPI_TransmitReceive(&hspi1, pCmd, pData, 1, SPI_TIMEOUT);
#endif
#if 0
        HAL_StatusTypeDef statusCmd;
        HAL_StatusTypeDef statusReg;
        statusCmd = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
        statusReg = HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT );
#endif
#if 1
        if ( HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT))
        	HAL_SPI_Receive(&hspi1, pReg, regSize, SPI_TIMEOUT );
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
	uint8_t cmd = W_REGISTER & addr;
	uint8_t *pCmd = &cmd;
	size_t cmdSize = sizeof(cmd);
	size_t valSize = sizeof(val);

	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_RESET);
	csnLow();
#if 0
	HAL_StatusTypeDef statusCmd;
	HAL_StatusTypeDef statusVal;
	statusCmd = HAL_SPI_Transmit(&hspi1, pWrite, writeSize, SPI_TIMEOUT);
	statusVal = HAL_SPI_Transmit(&hspi1, &val, valSize, SPI_TIMEOUT);
#endif
#if 1
	if (HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT)) {
		HAL_SPI_Transmit(&hspi1, &val, valSize, SPI_TIMEOUT);
	}
	csnHigh();
	//HAL_GPIO_WritePin(CSN1_GPIO_Port, CSN1_Pin, GPIO_PIN_SET);
#endif
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
void csnLow(){
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
void ceHigh(){
	HAL_GPIO_WritePin(CE_GPIO_Port, CE_Pin, GPIO_PIN_SET);
}
