/*
 * highLevelModule.h
 *
 *  Created on: 7 mar 2020
 *      Author: Khevenin
 */

#ifndef INC_HIGHLEVELMODULE_H_
#define INC_HIGHLEVELMODULE_H_

#include <main.h>
#include <stdint.h>
#include <stddef.h>
#include "regCmd.h"



/* Structures */
typedef struct {
	uint8_t dataReadIrq :1;
	uint8_t dataSendIrq :1;
	uint8_t maxRetr :1;
	uint8_t pipeNumber :3;
	uint8_t txFull :1;

	uint8_t packetsLost;
	uint8_t packetsRetr;
} statusStruct_t;

typedef struct {
	uint8_t rxMode :1;
	uint8_t channel;
	dataRate_t dataRate;
	powerRF_t powerRF;

	uint8_t ard;	//automatic retransmissions
	uint8_t arc;	//auto retransmit count

	uint8_t pipeEn;
	uint8_t pipeACK;
	uint8_t pipeDPL;
	uint8_t pipePayLen[6];

	uint8_t enableCRC :1;
	uint8_t codingCRC :1;

	uint8_t enableTxIrq :1;
	uint8_t enableRxIrq :1;
	uint8_t enableMaxRtIrq :1;

	uint8_t enableDPL :1;
	uint8_t enableAckPay :1;
	uint8_t enableDynACK :1;
} settingsStruct_t;

typedef struct {
	addressWidth_t addrWidth;
	uint8_t txAddr[5];	//5 byte register

	uint8_t rxAddr0[5];	//5 byte register
	uint8_t rxAddr1[5];	//5 byte register
	uint8_t rxAddr2;	//1 byte registers
	uint8_t rxAddr3;
	uint8_t rxAddr4;
	uint8_t rxAddr5;
} addrresStruct_t;

typedef struct {
	uint8_t txReUse :1;

	uint8_t rxRead :1;
	uint8_t rxFull :1;
	uint8_t rxEmpty :1;

	uint8_t txSend :1;
	uint8_t txFull :1;
	uint8_t txEmpty :1;
} fifoStruct_t;

typedef struct {
	settingsStruct_t setStruct;
	addrresStruct_t addrStruct;
	fifoStruct_t fifoStruct;
	statusStruct_t statusStruct;

	SPI_HandleTypeDef *nRFspi;
	TIM_HandleTypeDef *nRFtim;

	GPIO_TypeDef *nRFportCSN;
	uint16_t nRFpinCSN;
	GPIO_TypeDef *nRFportCE;
	uint16_t nRFpinCE;
} nrfStruct_t;

nrfStruct_t* nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
		GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
		GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE);

void delayUs(nrfStruct_t *nrfStruct, uint16_t time);

/* CE snd CSN control funtions's */
void csnLow(nrfStruct_t *nrfStruct);  //TODO: in future create macro
void csnHigh(nrfStruct_t *nrfStruct); //TODO: in future create macro
void ceLow(nrfStruct_t *nrfStruct);   //TODO: in future create macro
void ceHigh(nrfStruct_t *nrfStruct);  //TODO: in future create macro

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
uint8_t writeTxPayloadAck(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayloadNoAck(nrfStruct_t *nrfStruct, uint8_t *buf,
		size_t bufSize);
/* Read payload lenght when DPL enable */
uint8_t readDynamicPayloadWidth(nrfStruct_t *nrfStruct);
/* Flush functions */
uint8_t flushTx(nrfStruct_t *nrfStruct);
uint8_t flushRx(nrfStruct_t *nrfStruct);
/* Others */
void reuseTxPayload(nrfStruct_t *nrfStruct);
uint8_t getStatus(nrfStruct_t *nrfStruct);

/* Power control */
void pwrUp(nrfStruct_t *nrfStruct);
void pwrDown(nrfStruct_t *nrfStruct);

uint8_t readBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);
void resetBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);
void setBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);


#endif /* INC_HIGHLEVELMODULE_H_ */
