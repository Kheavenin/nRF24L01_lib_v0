/*
 * highLevelModule.c
 *
 *  Created on: 7 mar 2020
 *      Author: Khevenin
 */

#include "highLevelModule.h"

void fifoStruct_Init() {

}

void addressStruct_Init() {

}

void settingStruct_Init() {

}

nrfStruct_t* nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
		GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
		GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE) {

	/* Create main nRF structure	 */
	static nrfStruct_t nRFMainStruct;
	static nrfStruct_t *pnRFMainStruct = &nRFMainStruct;

	/* Init settigns struct */
	nRFMainStruct.setStruct.rxMode = 0;			//set as receiver
	nRFMainStruct.setStruct.channel = 0; 				//set channel np. 0
	nRFMainStruct.setStruct.dataRate = RF_DataRate_2M;  //lowest data rate
	nRFMainStruct.setStruct.powerRF = RF_PWR_0dBm;		//-12dBm power

	nRFMainStruct.setStruct.ard = 0;		//auto retr. delay 250us
	nRFMainStruct.setStruct.arc = 3;		//auto retr. counter
	/* Pipe Enable - defult pipe 0 enable only */
	uint8_t i;
	for (i = 0; i < 6; i++) {
		if (i == 0 || i == 1) {
			nRFMainStruct.setStruct.pipeEn[i] = 1;
		}
		nRFMainStruct.setStruct.pipeEn[i] = 0;
	}
	/* Pipe ACK enable - default pipe 0 ACK enable only */
	for (i = 0; i < 6; i++) {
		nRFMainStruct.setStruct.pipeACK[i] = 1;
	}
	/* Pipe Dynamic Payload enable - default pipe 0 DPL enable only */
	for (i = 0; i < 6; i++) {
		nRFMainStruct.setStruct.pipeDPL[i] = 0;
	}
	/* Pipe RX Payload Lenght  */
	for (i = 0; i < 6; i++) {
		nRFMainStruct.setStruct.pipePayLen[i] = 0;
	}

	nRFMainStruct.setStruct.enableDPL = 0;
	nRFMainStruct.setStruct.enableAckPay = 0;
	nRFMainStruct.setStruct.enableDynACK = 0;	//enable NO_ACK command

	/* Init address struct */
	for (i = 0; i < 5; i++) {
		nRFMainStruct.addrStruct.txAddr[i] = DF_TX_ADDR_0;
	}
	for (i = 0; i < 5; ++i) {
		nRFMainStruct.addrStruct.rxAddr0[i] = DF_RX_ADDR_P0_0;
	}
	for (i = 0; i < 5; ++i) {
		nRFMainStruct.addrStruct.rxAddr1[i] = DF_RX_ADDR_P1_0;
	}
	nRFMainStruct.addrStruct.rxAddr2 = DF_RX_ADDR_P2;
	nRFMainStruct.addrStruct.rxAddr3 = DF_RX_ADDR_P3;
	nRFMainStruct.addrStruct.rxAddr4 = DF_RX_ADDR_P4;
	nRFMainStruct.addrStruct.rxAddr5 = DF_RX_ADDR_P5;

	/* Init fifo struct */
	nRFMainStruct.fifoStruct.rxRead = 0;
	nRFMainStruct.fifoStruct.rxFull = 0;
	nRFMainStruct.fifoStruct.rxEmpty = 1;

	nRFMainStruct.fifoStruct.txSend = 0;
	nRFMainStruct.fifoStruct.txFull = 0;
	nRFMainStruct.fifoStruct.txEmpty = 1;

	/* Put pointer of SPI and TIM structures to nRF alias */
	pnRFMainStruct->nRFspi = HAL_SPIx;
	pnRFMainStruct->nRFtim = HAL_TIMx;

	/* Put GPIO port pointer and pin number to structure's alias */
	pnRFMainStruct->nRFportCSN = HAL_GPIO_CSN;
	pnRFMainStruct->nRFpinCSN = HAL_GPIO_Pin_CSN;
	pnRFMainStruct->nRFportCE = HAL_GPIO_CE;
	pnRFMainStruct->nRFpinCE = HAL_GPIO_Pin_CE;

	return pnRFMainStruct;

}

