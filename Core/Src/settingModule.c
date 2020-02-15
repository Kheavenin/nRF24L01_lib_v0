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
	if (pipe >= 0 && pipe <= 5) {
		return 1;
	}
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

