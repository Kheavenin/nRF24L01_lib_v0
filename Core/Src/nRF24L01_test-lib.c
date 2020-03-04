/*
 * nRF24L01_test-lib.c
 *
 *  Created on: 22 lut 2020
 *      Author: Khevenin
 */

#include "nRF24L01_test-lib.h"

bool test_RFchannel(void)
{
	uint8_t inputDataStatus = OK_CODE;
	uint16_t fallCounter = 0;
	// Correct input data
	uint8_t var = 0;
	uint8_t max = 125;
	for (var = 0; var < max; var++) {
		inputDataStatus = setChannel(var);
		if (inputDataStatus == FALL)
			fallCounter++;
	}
	//check errors
	if (fallCounter > 0)
		return FALL;

	//Incorrect input data
	inputDataStatus = OK_CODE;
	fallCounter = 0;
	max = 255;
	for (uint16_t i = 126; i < max; i++) {
		inputDataStatus = setChannel(i);
		if (inputDataStatus == FALL)
			fallCounter++;
	}
	//check errors
	if (fallCounter == 0)		//methode didn't detect error data
		return FALL;

	//check write to register
	inputDataStatus = OK_CODE;
	fallCounter = 0;
	max = 125;
	uint8_t readVal = 0;
	for (var = 0; var < max; var++) {
		inputDataStatus = setChannel(var);
		readVal = readRegister(RF_CH);
		if (var != readVal) {
			fallCounter++;
		}
	}
	//check errors
	if (fallCounter == 0)		//All read values are correct
		return PASS;
	/* If not or something is wrong with test */
	return FALL;
}

bool test_RFpower(void)
 {		// Correct input data
	uint16_t fallCounter = 0;
	uint8_t tmp = 0;
	uint8_t var;
	uint8_t max = 3;
	for (var = 0; var <= max; var++) {
		setRFpower(var);
		tmp = readRegister(RF_SETUP);
		tmp = ((tmp >> 1) & 0x03);
		if (tmp != var)
			fallCounter++;
	}
	//check errors
	if (fallCounter > 0)
		return FALL;
	return PASS;
}

bool test_RFdataRate(void) {
	// Correct input data
	uint16_t fallCounter = 0;
	uint8_t tmp = 0;

	setRFpower(RF_DataRate_250);
	tmp = readRegister(RF_SETUP);
	tmp = ((tmp >> 3) & 0x2E); //0000 0101B
	if (tmp != RF_DataRate_250)
		fallCounter++;

	setRFpower(RF_DataRate_1M);
	tmp = readRegister(RF_SETUP);
	tmp = ((tmp >> 3) & 0x2E); //0000 0101B
	if (tmp != RF_DataRate_1M)
		fallCounter++;

	setRFpower(RF_DataRate_2M);
	tmp = readRegister(RF_SETUP);
	tmp = ((tmp >> 3) & 0x2E); //0000 0101B
	if (tmp != RF_DataRate_2M)
		fallCounter++;

	//check errors
	if (fallCounter > 0)
		return FALL;
	return PASS;
}

bool test_ReciveAddress(void) {
	uint8_t buf[] = { 'a', 'b', 'c', 'd', 'e' };
	uint8_t *addrBuf = buf;
	uint8_t addrBufSize = sizeof(buf);
	setAddrWidth(longWidth); //set lenght of address as 5 bytes

	uint8_t var;
	uint8_t max = 6;
	uint8_t inputDataStatus = OK_CODE;
	uint16_t fallCounter = 0;

	for (var = 0; var < max; var++) {
		inputDataStatus = setReceivePipeAddress(var, addrBuf, addrBufSize);
		if (inputDataStatus == ERR_CODE)
			fallCounter++;
	}
	if (fallCounter > 0)
		return FALL;

	max = 64;
	inputDataStatus = OK_CODE;
	fallCounter = 0;
	for (var = 0; var < max; var++) {
		inputDataStatus = setReceivePipeAddress(var, addrBuf, addrBufSize);
		if (inputDataStatus == ERR_CODE)
			fallCounter++;
	}
	if (fallCounter == 0)
		return FALL;


	max = 6;
	inputDataStatus = OK_CODE;
	fallCounter = 0;
	uint8_t testBuf[5];

	for (var = 0; var < max; var++) {
		inputDataStatus = setReceivePipeAddress(var, addrBuf, addrBufSize);
		uint8_t addr = 0x0A + var;
		multiRead(addr, testBuf, sizeof(testBuf));
	if(buf[4] != testBuf[4] && buf[3] != testBuf[3])
			fallCounter++;
	}
	if (fallCounter == 0) {
		return PASS;
	}
	return FALL;
}

bool test_TransmitAddress(void) {
	uint8_t buf[] = { 'a', 'b', 'c', 'd', 'e' };
	uint8_t *addrBuf = buf;

	uint16_t fallCounter = 0;
	uint8_t inputDataStatus = OK_CODE;
	uint8_t var;
	uint8_t max = 10;

	for (var = 0; var < max; var++) {
		inputDataStatus = setTransmitPipeAddress(addrBuf, var);
		if (inputDataStatus == ERR_CODE)
			fallCounter++;
	}
	if (fallCounter < 9) {
		return FALL;
	}
	uint8_t testBuf[5];
	multiRead(TX_ADDR, testBuf, sizeof(testBuf));
	if (buf[4] != testBuf[4] && buf[3] != testBuf[3]) {
		return FALL;
	}
	return PASS;
}

/* tests of read/write registers */

bool test_ReadWriteSingleRegisters() {
	powerUp();
	const size_t testSize = 10;
	uint8_t countPass = 0;
	bool testTab[testSize];
	/* tab of functions's pointers*/
	bool (*testFunTab[])(
			void) = {test_Config, test_EN_AA, test_EN_RXADR, test_SETUP_AW, test_SETUP_RETR, test_RF_SETUP,
				test_STATUS, test_RX_PW, test_DYNPD, test_FEATURE,
	};
	/* Config test */
	uint8_t i = 0;
	for (i = 0; i < testSize; i++) {
		testTab[i] = (testFunTab[i])();	//result of test return to tab
	}

	for (i = 0; i < testSize; i++) {
		if (testTab[i] == 1) {
			countPass++;
		}
	}
	if (countPass == testSize)
		return PASS;
	return FALL;

}


bool test_Config() {
	uint8_t counter = 0;
	uint8_t max = 0x7F;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i | 0x02;
		writeRegister(CONFIG, var);
		if (readRegister(CONFIG) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_EN_AA() {
	uint8_t counter = 0;
	uint8_t max = 0x3F;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i;
		writeRegister(EN_AA, var);
		if (readRegister(EN_AA) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_EN_RXADR() {
	uint8_t counter = 0;
	uint8_t max = 0x3F;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i;
		writeRegister(EN_RXADDR, var);
		if (readRegister(EN_RXADDR) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_SETUP_AW() {
	uint8_t counter = 0;
	uint8_t max = 0x03;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0x01; i < max; i++) {
		var = i;
		writeRegister(SETUP_AW, var);
		if (readRegister(SETUP_AW) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_SETUP_RETR() {
	uint8_t counter = 0;
	uint8_t max = 0xfF;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i;
		writeRegister(SETUP_RETR, var);
		if (readRegister(SETUP_RETR) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_RF_SETUP() {
	uint8_t counter = 0;
	uint8_t max = 0xFF;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i & 0xBF;
		writeRegister(RF_SETUP, var);
		if (readRegister(RF_SETUP) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_STATUS() {
	uint8_t counter = 0;
	uint8_t max = 0x7F;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i & 0xF0;
		writeRegister(STATUS, var);
		if (readRegister(STATUS) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;

}

/* Observe TX register can be test only with full RF transmission */
/* RPD register can be test only with full RF transmission */

bool test_RX_PW() {
	uint8_t counter = 0;
	uint8_t counterPipe = 0;
	uint8_t pipe = 5;
	uint8_t max = 0x3F;
	uint8_t i, j;
	uint8_t addr = RX_ADDR_P0;
	uint8_t addrMax = RX_ADDR_P0 + pipe;
	uint8_t var = 0;

	for (j = 0; j < addrMax; j++) {
		for (i = 0; i < max; i++) {
			var = i;
			writeRegister(addr, var);
			if (readRegister(addr) == var) {
				counter++;
			}
		}
		if (counter == max)
			counterPipe++;
	}
	if (counterPipe == 5)
		return PASS;
	return FALL;
}
/* FIFO status register can be test only with full RF transmission */

bool test_DYNPD() {
	uint8_t counter = 0;
	uint8_t max = 0x3F;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i;
		writeRegister(DYNPD, var);
		if (readRegister(DYNPD) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_FEATURE() {
	uint8_t counter = 0;
	uint8_t max = 0x07;
	uint8_t i;
	uint8_t var = 0;

	for (i = 0; i < max; i++) {
		var = i;
		writeRegister(FEATURE, var);
		if (readRegister(FEATURE) == var) {
			counter++;
		}
	}
	if (counter == max)
		return PASS;
	return FALL;
}

bool test_RxPayload() {
	return ERR_CODE;
}

bool test_TxPayload() {
	size_t sizesBuffer[] = { 8, 12, 16, 20, 24 };
	size_t sizeSizes = sizeof(sizesBuffer);

	static size_t size32 = 32;
	uint8_t buffer[size32];
	uint8_t i;
	uint8_t max = size32;

	uint16_t passCounter = 0;

	/* prepare buffer with data */
	for (i = 0; i < max; ++i) {
		buffer[i] = i;
	}

	for (i = 0; i < sizeSizes; i++) {
		writeTxPayload(buffer, sizesBuffer[i]);
		if (!readBit(FIFO_STATUS, bit4))
			passCounter++;
		flushTx();
	}

	writeTxPayload(buffer, size32);
	if (readBit(FIFO_STATUS, bit5))
		passCounter++;
	
	if (passCounter == sizeSizes + 1)
		return PASS;
	return FALL;

}

