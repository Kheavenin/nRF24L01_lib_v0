/*
 * nRF24L01_test-lib.h
 *
 *  Created on: 22 lut 2020
 *      Author: Khevenin
 */

#ifndef INC_NRF24L01_TEST_LIB_H_
#define INC_NRF24L01_TEST_LIB_H_

/* Include block */
#include "settingModule.h"
#include <stdbool.h>
/* Directives block */
#define PASS 0x01
#define FALL 0xFF

bool test_RFchannel(void);
bool test_RFpower(void);
bool test_RFdataRate(void);


#endif /* INC_NRF24L01_TEST_LIB_H_ */
