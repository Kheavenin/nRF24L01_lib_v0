/*
 * highLevelModule.h
 *
 *  Created on: 7 mar 2020
 *      Author: Khevenin
 */

#ifndef INC_HIGHLEVELMODULE_H_
#define INC_HIGHLEVELMODULE_H_

#include "settingModule.h"

nrfStruct_t* nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
		GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
		GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE);




/* CE snd CSN control funtions's */
void csnL(nrfStruct_t *nrfStruct);  //TODO: in future create macro
void csnH(nrfStruct_t *nrfStruct); //TODO: in future create macro
void ceL(nrfStruct_t *nrfStruct);   //TODO: in future create macro
void ceH(nrfStruct_t *nrfStruct);  //TODO: in future create macro

/* Elementary functions base on nRf24L01+ SPI commands */
/* Read and write registers funtions's */
uint8_t readReg(nrfStruct_t *nrfStruct, uint8_t addr);
void writeReg(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t val);
/* Extended read and write functions - R/W few registers */
void readRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
		size_t bufSize);
void writeRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
		size_t bufSize);

/* Payload's functions */
uint8_t readRxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t readRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize,
		uint8_t width);
uint8_t writeTxPayloadAck(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayloadNoAck(nrfStruct_t *nrfStruct, uint8_t *buf,
		size_t bufSize);
/* Flush functions */
uint8_t flushTx(nrfStruct_t *nrfStruct);
uint8_t flushRx(nrfStruct_t *nrfStruct);
/* Others */
void reuseTxPayload(nrfStruct_t *nrfStruct);
uint8_t getStatus(nrfStruct_t *nrfStruct);

/* Power control */
void pwrUp(nrfStruct_t *nrfStruct);
void pwrDown(nrfStruct_t *nrfStruct);



















#endif /* INC_HIGHLEVELMODULE_H_ */
