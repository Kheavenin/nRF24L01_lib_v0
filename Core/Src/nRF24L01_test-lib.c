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

