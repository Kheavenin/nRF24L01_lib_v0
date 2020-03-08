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
	nRFMainStruct.setStruct.rxMode = 1;			//set as receiver
	nRFMainStruct.setStruct.channel = 0; 				//set channel np. 0
	nRFMainStruct.setStruct.dataRate = RF_DataRate_250;  //lowest data rate
	nRFMainStruct.setStruct.powerRF = RF_PWR_12dBm;		//-12dBm power

	nRFMainStruct.setStruct.ard = 0;		//auto retr. delay 250us
	nRFMainStruct.setStruct.arc = 3;		//auto retr. counter
	/* Pipe Enable - defult pipe 0 enable only */
	uint8_t i;
	for (i = 0; i < 6; i++) {
		if (i == 0) {
			nRFMainStruct.setStruct.pipeEn[i] = 1;
		}
		nRFMainStruct.setStruct.pipeEn[i] = 0;
	}
	/* Pipe ACK enable - default pipe 0 ACK enable only */
	for (i = 0; i < 6; i++) {
		if (i == 0) {
			nRFMainStruct.setStruct.pipeACK[i] = 1;
		}
		nRFMainStruct.setStruct.pipeACK[i] = 0;
	}
	/* Pipe Dynamic Payload enable - default pipe 0 DPL enable only */
	for (i = 0; i < 6; i++) {
		if (i == 0) {
			nRFMainStruct.setStruct.pipeDPL[i] = 1;
		}
		nRFMainStruct.setStruct.pipeDPL[i] = 0;
	}
	/* Pipe RX Payload Lenght  */
	for (i = 0; i < 6; i++) {
		nRFMainStruct.setStruct.pipePayLen[i] = 0;
	}

	nRFMainStruct.setStruct.enableDPL = 1;
	nRFMainStruct.setStruct.enableAckPay = 1;
	nRFMainStruct.setStruct.enableDynACK = 1;	//enable NO_ACK command

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

