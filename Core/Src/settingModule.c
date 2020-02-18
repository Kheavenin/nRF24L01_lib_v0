#include "settingModule.h"

/**
 * @Brief	Switch radio module to Receiver (PRX) mode
 * @Retval	None
 */
void modeRX() {
	ceHigh(); //set high on CE line
	setBit(CONFIG, bit0);
}

/**
 *@Brief	Switch radio module to Transmitter (PTX) mode
 */
void modeTX() {
	ceHigh();
	resetBit(CONFIG, bit0);
}

/**
 * @Brief	Switch radio module to Standby-I mode
 */
void modeStandby() {
	ceLow();
}

/* Interrupts functions */
void disableRXinterrupt() {
	resetBit(CONFIG, bit6);	//disable RX_IRQ in Config register
}
void disableTXinterrupt() {
	resetBit(CONFIG, bit5);
}
void disableRTinterrupt() {
	resetBit(CONFIG, bit4);
}

void enableRXinterrupt() {
	setBit(CONFIG, bit6);
}
void enableTXinterrupt() {
	setBit(CONFIG, bit5);
}
void enableRTinterrupt() {
	setBit(CONFIG, bit4);
}

void clearRX_DR() {	//clear irt bits in Status Register
	setBit(STATUS, bit6);
}
void clearTX_DS() {
	setBit(STATUS, bit5);
}
void clearMAX_RT() {
	setBit(STATUS, bit4);
}


/* CRC functions */
void enableCRC() {
	setBit(CONFIG, bit3);
}
void setCRC(widthCRC_t w) {
	if (w) {
		setBit(CONFIG, bit2);
	} else {
		resetBit(CONFIG, bit2);
	}
}

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe) {
	if (pipe >= 0 && pipe <= 5)	//check correct pipe number
		return 1;
	return 0;
}
uint8_t enableAutoAckPipe(uint8_t pipe) {
	if (checkPipe(pipe)) {
		setBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}
uint8_t disableAutoAckPipe(uint8_t pipe) {
	if (checkPipe(pipe)) {
		resetBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}

/* RX addresses */
uint8_t enableRxAddr(uint8_t pipe) {
	if (checkPipe(pipe)) {
		setBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}
uint8_t disableRxAddr(uint8_t pipe) {
	if (checkPipe(pipe)) {
		resetBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}

/* Address Width */
void setAddrWidth(addressWidth_t width) {
	writeRegister(SETUP_AW, width);
}

/* Setup retransmission */
uint8_t setAutoRetrCount(uint8_t count) {
	if (count >= 0x00 && count <= 0x0F) {			//check count val
		uint8_t tmp = readRegister(SETUP_RETR);		//read reg. val
		tmp = tmp & 0xF0;	// reset LSB and save MSB
		tmp |= count;		//add tmp and count
		writeRegister(SETUP_RETR, tmp);				//write to SETUP_RETR
		return 1;
	}
	return 0;
}

uint8_t setAutoRetrDelay(uint8_t delay) {
	if (delay > 0x0F) {						//if delay in MSB format
		delay = delay >> 8;					//shift to LSB format
	}
	if (delay >= 0x00 && delay <= 0x0F) {
		uint8_t tmp = readRegister(SETUP_RETR);
		tmp = tmp & 0x0F;		//save LSB, reset MSB
		tmp |= (delay << 8);	//add tmp and delay
		writeRegister(SETUP_RETR, tmp);
		return 1;
	}
	return 0;
}

/* RF channel */
uint8_t setChannel(uint8_t channel) {
	if (channel >= 0 && channel <= 125) {
		writeRegister(RF_CH, channel);//Maximum channel limited to 125 by hardware
		return 1;
	}
	return 0;
}

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier() {
	setBit(RF_SETUP, bit7);
}
void disableContCarrier() {
	resetBit(RF_SETUP, bit7);
}
void enableLockPLL() {
	setBit(RF_SETUP, bit4);
}
void diableLockPLL() {
	resetBit(RF_SETUP, bit4);
}


void setRFpower(powerRF_t power) {
	uint8_t tmp = readRegister(RF_SETUP);		//
	tmp = tmp & 0xF8;							//0xF8 - 1111 1000B reset 3 LSB
	tmp = tmp | (power << 1);				//combining tmp and shifted power
	writeRegister(RF_SETUP, tmp);
}

void setDataRate(dataRate_t rate) {
	uint8_t tmp = readRegister(RF_SETUP);			//
	tmp = tmp & 0x06; //0x86 = 0000 0110B - reset data rate's bits - Also this line reset PLL_LOCK and CONT_WAVE bits
	tmp = tmp | (rate << 3);		//combining tmp and shifted data rate
	writeRegister(RF_SETUP, tmp);
}

/* Status */
uint8_t checkTxFIFO() {
	if (readBit(STATUS, bit0))
		return 0; //TX FIFO full
	else
		return 0;
}
uint8_t checkRxFIFO() {	//TODO: pomyslec nad kodem zwracanym gdy RX FIFO empty
	uint8_t tmp = readRegister(STATUS);
	tmp &= 0x0E;	//save only pipe number bits
	tmp = tmp >> 1;
	if (checkPipe(tmp))
		return tmp;
	if (tmp == 0x07)		//RX FIFO empty
		return 7;
	if (tmp == 0x06)	//110B - mean not used
		return -1;		//return ERR
	return -1;
}

/* Transmit observe */
uint8_t lostPacketsCount() {
	uint8_t tmp = readRegister(OBSERVE_TX);
	return (tmp >> 4);
}

uint8_t retrPacketsCount() {
	uint8_t tmp = readRegister(OBSERVE_TX);
	return (tmp & 0xF0);
}

/* RPD - for test use only */
uint8_t checkRPD() {
	if (readRegister(RPD))
		return 1;
	else
		return 0;
}


/* Receive Address data pipe */
/**
 * @Brief	Write receiver address of pipe
 * @Param	Number of pipe
 * @Param	Pointer to buffer with address
 * @Param	Size of address's buffer
 * @Note	Remember that addresses registers for pipes from 2 to 5 are 1 byte only.
 * 			Also registers for pipe 0 and 1 can have size of from 3 to 5 bytes.
 */
uint8_t setReceivePipeAddress(uint8_t pipe, uint8_t *addrBuf,
		size_t addrBufSize) {
	if (!checkPipe(pipe)) {	//if checkPipe return 0 - end fun. by return 0.
		return 0;
	}
	uint8_t addr = RX_ADDR_P0 + pipe;//if pipe = 0 -> write Receive address pipe 0
	if (pipe >= 2 && pipe <= 5) {
		if (addrBufSize != 1) {
			return 0;
		}
	}
	multiWrite(addr, addrBuf, addrBufSize);
	return 1;
}

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(uint8_t *addrBuf, size_t addrBufSize) {
	if (addrBufSize != 5) {
		return 0;					//if addrBufSize isn't 5 bytes retun 0
	}
	multiWrite(TX_ADDR, addrBuf, addrBufSize);
	return 1;
}


/* RX Payload width */
uint8_t getRxPayload(uint8_t pipe) {	//TODO: to tests
	if (checkPipe(pipe)) {
		addr = RX_PW_P0 + pipe;
		return readRegister(addr);
	}
	return 0;
}

uint8_t setRxPayload(uint8_t pipe, uint8_t width) { //TODO: to tests
	if (checkPipe(pipe)) {
		if (width < 1 && > 32) {	//check width correct value
			return 0;
		}
		addr = RX_PW_P0 + pipe;
		writeRegister(addr, width);
		return 1;
	}
}














