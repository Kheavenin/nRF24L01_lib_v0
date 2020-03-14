/*
 * highLevelModule.c
 *
 *  Created on: 7 mar 2020
 *      Author: Khevenin
 */

#include "highLevelModule.h"

void settingStruct_Init(nrfStruct_t *nrfStruct) {
	/* Init settigns struct */
	nrfStruct->setStruct.rxMode = 0;			//set as receiver
	nrfStruct->setStruct.channel = 0; 				//set channel np. 0
	nrfStruct->setStruct.dataRate = RF_DataRate_2M;  //lowest data rate
	nrfStruct->setStruct.powerRF = RF_PWR_0dBm;		//-12dBm power

	nrfStruct->setStruct.ard = 0;		//auto retr. delay 250us
	nrfStruct->setStruct.arc = 3;		//auto retr. counter
	/* Pipe Enable - defult pipe 0 enable only */
	uint8_t i;
	for (i = 0; i < 6; i++) {
		if (i == 0 || i == 1)
			nrfStruct->setStruct.pipeEn[i] = 1;
		else
			nrfStruct->setStruct.pipeEn[i] = 0;
	}
	/* Pipe ACK enable - default pipe 0 ACK enable only */
	for (i = 0; i < 6; i++) {
		nrfStruct->setStruct.pipeACK[i] = 1;
	}
	/* Pipe Dynamic Payload enable - default pipe 0 DPL enable only */
	for (i = 0; i < 6; i++) {
		nrfStruct->setStruct.pipeDPL[i] = 0;
	}
	/* Pipe RX Payload Lenght  */
	for (i = 0; i < 6; i++) {
		nrfStruct->setStruct.pipePayLen[i] = 0;
	}

	nrfStruct->setStruct.enableDPL = 0;
	nrfStruct->setStruct.enableAckPay = 0;
	nrfStruct->setStruct.enableDynACK = 0;	//enable NO_ACK command
}

void addressStruct_Init(nrfStruct_t *nrfStruct) {
	/* Init address struct */
	uint8_t i;
	for (i = 0; i < 5; i++) {
		nrfStruct->addrStruct.txAddr[i] = DF_TX_ADDR_0;
	}
	for (i = 0; i < 5; ++i) {
		nrfStruct->addrStruct.rxAddr0[i] = DF_RX_ADDR_P0_0;
	}
	for (i = 0; i < 5; ++i) {
		nrfStruct->addrStruct.rxAddr1[i] = DF_RX_ADDR_P1_0;
	}
	nrfStruct->addrStruct.rxAddr2 = DF_RX_ADDR_P2;
	nrfStruct->addrStruct.rxAddr3 = DF_RX_ADDR_P3;
	nrfStruct->addrStruct.rxAddr4 = DF_RX_ADDR_P4;
	nrfStruct->addrStruct.rxAddr5 = DF_RX_ADDR_P5;
}

void fifoStruct_Init(nrfStruct_t *nrfStruct) {
	/* Init fifo struct */
	nrfStruct->fifoStruct.rxRead = 0;
	nrfStruct->fifoStruct.rxFull = 0;
	nrfStruct->fifoStruct.rxEmpty = 1;

	nrfStruct->fifoStruct.txSend = 0;
	nrfStruct->fifoStruct.txFull = 0;
	nrfStruct->fifoStruct.txEmpty = 1;
}

void hardware_Init(nrfStruct_t *nrfStruct, SPI_HandleTypeDef *HAL_SPIx,
		TIM_HandleTypeDef *HAL_TIMx, GPIO_TypeDef *HAL_GPIO_CSN,
		uint16_t HAL_GPIO_Pin_CSN, GPIO_TypeDef *HAL_GPIO_CE,
		uint16_t HAL_GPIO_Pin_CE) {
	/* Put pointer of SPI and TIM structures to nRF alias */
	nrfStruct->nRFspi = HAL_SPIx;
	nrfStruct->nRFtim = HAL_TIMx;

	/* Put GPIO port pointer and pin number to structure's alias */
	nrfStruct->nRFportCSN = HAL_GPIO_CSN;
	nrfStruct->nRFpinCSN = HAL_GPIO_Pin_CSN;
	nrfStruct->nRFportCE = HAL_GPIO_CE;
	nrfStruct->nRFpinCE = HAL_GPIO_Pin_CE;
}


nrfStruct_t* nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
		GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
		GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE) {

	/* Create main nRF structure	 */
	static nrfStruct_t nRFMainStruct;
	static nrfStruct_t *pnRFMainStruct = &nRFMainStruct;

	/* Init settigns struct */

	settingStruct_Init(pnRFMainStruct);
	addressStruct_Init(pnRFMainStruct);
	fifoStruct_Init(pnRFMainStruct);
	hardware_Init(pnRFMainStruct, HAL_SPIx, HAL_TIMx, HAL_GPIO_CSN,
			HAL_GPIO_Pin_CSN, HAL_GPIO_CE, HAL_GPIO_Pin_CE);


	return pnRFMainStruct;

}

