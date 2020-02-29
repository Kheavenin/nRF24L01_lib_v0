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
bool test_ReciveAddress(void);
bool test_TransmitAddress(void);

bool test_ReadWriteSingleRegisters();
bool test_Config();
bool test_EN_AA();
bool test_EN_RXADR();
bool test_SETUP_AW();
bool test_SETUP_RETR();
bool test_RF_SETUP();
bool test_STATUS();
bool test_RX_PW();
bool test_DYNPD();
bool test_FEATURE();

bool test_RxPayload();
bool test_TxPayload();

#endif /* INC_NRF24L01_TEST_LIB_H_ */
