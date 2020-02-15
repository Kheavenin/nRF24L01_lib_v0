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
