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

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);
	DelayUs(50);
	//HAL_Delay(1);
	HAL_SPI_Receive(&hspi1, pReg, sizeof(reg), SPI_TIMEOUT);
	/*
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
	 #endif */

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

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);
	//HAL_Delay(1);
	DelayUs(50);
	HAL_SPI_Transmit(&hspi1, &val, sizeof(val), SPI_TIMEOUT);

	/*
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
#endif
	 */
	csnHigh();
}

/* Multi bytes read/write register functions */
void multiRead(uint8_t addr, uint8_t *buf, size_t bufSize) {
	uint8_t cmd = R_REGISTER | addr;
	uint8_t *pCmd = &cmd;
	size_t cmdSize = sizeof(cmd);
	csnLow();

	HAL_StatusTypeDef statusRead;
	HAL_StatusTypeDef statusCmd;
	statusCmd = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
	HAL_Delay(1);
	statusRead = HAL_SPI_Receive(&hspi1, buf, bufSize, SPI_TIMEOUT);

	csnHigh();
}
void multiWrite(uint8_t addr, uint8_t *buf, size_t bufSize) {
	uint8_t cmd = W_REGISTER | addr;
	uint8_t *pCmd = &cmd;
	size_t cmdSize = sizeof(cmd);
	csnLow();

	HAL_StatusTypeDef statusSend;
	HAL_StatusTypeDef statusRead;
	statusSend = HAL_SPI_Transmit(&hspi1, pCmd, cmdSize, SPI_TIMEOUT);
	HAL_Delay(1);
	statusRead = HAL_SPI_Transmit(&hspi1, buf, bufSize, SPI_TIMEOUT);

	csnHigh();
}

/* Payload functions*/
uint8_t readRxPayload(uint8_t *buf, size_t bufSize) {
	if (bufSize < 1)
		return ERR_CODE;
	if (bufSize > 32)
		bufSize = 32;

	uint8_t cmd = R_RX_PAYLOAD;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	DelayUs(50);
	HAL_SPI_Receive(&hspi1, buf, bufSize, SPI_TIMEOUT);			//read payload

	csnHigh();
	return OK_CODE;
}

uint8_t writeTxPayload(uint8_t *buf, size_t bufSize) {
	if (bufSize < 1)
		return ERR_CODE;
	if (bufSize > 32)
		bufSize = 32;

	uint8_t cmd = W_TX_PAYLOAD;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	DelayUs(50);
	HAL_SPI_Transmit(&hspi1, buf, bufSize, SPI_TIMEOUT);		//read payload

	csnHigh();
	return OK_CODE;
}

uint8_t readRxPayloadWidth(uint8_t *buf, size_t bufSize, uint8_t width) {
	if (bufSize < 1)
		return ERR_CODE;	//invalid buffer size
	if (width < 1)
		return ERR_CODE;	//invaild number of bytes
	if (bufSize > 32)
		bufSize = 32;		//to big buffer size
	if (width > 32) 		//to bi width of fifo to read
		width = 32;
	if (bufSize < width)
		width = bufSize;

	uint8_t cmd = R_RX_PL_WID;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	DelayUs(50);
	HAL_SPI_Receive(&hspi1, buf, width, SPI_TIMEOUT);			//read payload

	csnHigh();
	return OK_CODE;
}

uint8_t writeTxPayloadAck(uint8_t *buf, size_t bufSize) {
	if (bufSize < 1)
		return ERR_CODE;
	if (bufSize > 32)
		bufSize = 32;

	uint8_t cmd = W_ACK_PAYLOAD;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	DelayUs(50);
	HAL_SPI_Transmit(&hspi1, buf, bufSize, SPI_TIMEOUT);		//read payload

	csnHigh();
	return OK_CODE;
}

uint8_t writeTxPayloadNoAck(uint8_t *buf, size_t bufSize) {
	if (bufSize < 1)
		return ERR_CODE;
	if (bufSize > 32)
		bufSize = 32;

	uint8_t cmd = W_TX_PAYLOAD_NO_ACK;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	DelayUs(50);
	HAL_SPI_Transmit(&hspi1, buf, bufSize, SPI_TIMEOUT);		//read payload

	csnHigh();
	return OK_CODE;
}

uint8_t flushTx() {
	uint8_t cmd = FLUSH_TX;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	//DelayUs(50);

	csnHigh();
	return OK_CODE;
}

uint8_t flushRx() {
	uint8_t cmd = FLUSH_RX;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	//DelayUs(50);

	csnHigh();
	return OK_CODE;
}

void reuseTxPayload() {
	uint8_t cmd = REUSE_TX_PL;	//set command mask
	uint8_t *pCmd = &cmd;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);	//send command
	//DelayUs(50);

	csnHigh();
}

/**
 * @Brief	Get Status Register.
 * @Retval	Return value of Status Register
 */
uint8_t getStatus() {
	uint8_t cmd = NOP;
	uint8_t *pCmd = &cmd;

	uint8_t reg = 0;
	uint8_t *pReg = &reg;

	csnLow();

	HAL_SPI_Transmit(&hspi1, pCmd, sizeof(cmd), SPI_TIMEOUT);
	DelayUs(50);
	HAL_SPI_Receive(&hspi1, pReg, sizeof(reg), SPI_TIMEOUT);

	csnHigh();
	return reg;
}

/**
 * @Brief	Read bit in register
 */
uint8_t readBit(uint8_t addr, bitNum_t bit) {
	uint8_t reg = readRegister(addr);
	return ((reg >> bit) & 0x01);
}

/**
 * @Brief	Set logic '1' on selected position
 */
void setBit(uint8_t addr, bitNum_t bit) {
	uint8_t tmp = readRegister(addr);
	tmp |= 1 << bit;
	writeRegister(addr, tmp);
}

/**
 * @Brief	Set logic '0' on selected position
 */
void resetBit(uint8_t addr, bitNum_t bit) {
	uint8_t tmp = readRegister(addr);
	tmp &= 0 << bit;		//zmieniono OR na AND
	writeRegister(addr, tmp);
}

/**
 * @Brief	Turn on nRF24L01+ module
 */
void powerUp() {
	setBit( CONFIG, PWR_UP);
}

/**
 * @Brief Turn off nRF24L01+ module
 */
void powerDown() {
	resetBit(CONFIG, PWR_UP);
}

/**
 * @Brief	Set low level on CSN line
 */
void csnLow() {
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_RESET);
}

/**
 * @Brief	Set high level on CSN line
 */
void csnHigh() {
	HAL_GPIO_WritePin(CSN_GPIO_Port, CSN_Pin, GPIO_PIN_SET);
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

void DelayUs(uint16_t time) {

	__HAL_TIM_SET_COUNTER(&htim1, 0);	//Set star value as 0
	while (__HAL_TIM_GET_COUNTER(&htim1) < time)
		;

}
