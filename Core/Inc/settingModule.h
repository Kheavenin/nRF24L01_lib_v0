#include <main.h>
#include <stdint.h>
#include <stddef.h>

//#include "controleModule.h"

/* Structures */
typedef struct {
	uint8_t dataReadIrq :1;
	uint8_t dataSendIrq :1;
	uint8_t maxRetr :1;
	uint8_t pipeNumber :3;
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
	uint8_t txAddr[5];	//5 byte register

	uint8_t rxAddr0[5];	//5 byte register
	uint8_t rxAddr1[5];	//5 byte register
	uint8_t rxAddr2;	//1 byte registers
	uint8_t rxAddr3;
	uint8_t rxAddr4;
	uint8_t rxAddr5;
} addrresStruct_t;

typedef struct {
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

/* Modes */
void modeRX(nrfStruct_t *nrfStruct);
void modeTX(nrfStruct_t *nrfStruct);
void modeStandby(nrfStruct_t *nrfStruct);

/* Interrupts */
void disableTXinterrupt(nrfStruct_t *nrfStruct); //mask data send interrupt
void disableRXinterrupt(nrfStruct_t *nrfStruct); //mask data receive interrupt
void disableMaxRTinterrupt(nrfStruct_t *nrfStruct); //mask data retransmition interrupt

void enableTXinterrupt(nrfStruct_t *nrfStruct); //dont mask data send interrupt
void enableRXinterrupt(nrfStruct_t *nrfStruct); //dont mask data receive interrupt
void enableRTinterrupt(nrfStruct_t *nrfStruct); //dont mask data retransmition interrupt

void clearRX_DR(nrfStruct_t *nrfStruct); //clear irt bits in Status Register
void clearTX_DS(nrfStruct_t *nrfStruct);
void clearMAX_RT(nrfStruct_t *nrfStruct);

/* CRC */
void enableCRC(nrfStruct_t *nrfStruct);
void setCRC(nrfStruct_t *nrfStruct, widthCRC_t w);

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe);
uint8_t enableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);

/* RX addresses */
uint8_t enableRxAddr(uint8_t pipe);
uint8_t disableRxAddr(uint8_t pipe);

/* Address Width */
void setAddrWidth(addressWidth_t width);

/* Setup retransmission */
uint8_t setAutoRetrCount(uint8_t count);
uint8_t setAutoRetrDelay(uint8_t delay);

/* RF channel */
uint8_t setChannel(uint8_t channel);

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier();
void disableContCarrier();
void enableLockPLL();
void diableLockPLL();

void setRFpower(powerRF_t power);
void setDataRate(dataRate_t rate); //TODO: to test

/* Status */
uint8_t getStatusFullTxFIFO(); //TODO: to tests
uint8_t getPipeStatusRxFIFO(); //TODO: to tests

/* Transmit observe */
uint8_t lostPacketsCount(); //TODO: to tests
uint8_t retrPacketsCount(); //TODO: to tests

/* RPD - for RF test use only */
uint8_t checkRPD();

/* Receive Address data pipe */
uint8_t setReceivePipeAddress(uint8_t pipe, uint8_t *addr, size_t addrBufSize); //TODO: to tests

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(uint8_t *addrBuf, size_t addrBufSize); //TODO: to tests

/* Payload width of each pipes */
uint8_t getRxPayloadWidth(uint8_t pipe);				//TODO: to tests
uint8_t setRxPayload(uint8_t pipe, uint8_t width);		//TODO: to tests

/* TX and RX FIFO */
uint8_t getkRxStatusFIFO(); //TODO: to tests
uint8_t getTxStatusFIFO();  //TODO: to tests
uint8_t getTxReuse();       //TODO: to tests

/* DYNPD */
uint8_t enableDynamicPayloadLengthPipe(uint8_t pipe);  //TODO: to tests
uint8_t disableDynamicPayloadLengthPipe(uint8_t pipe); //TODO: to tests

/* Feature */
void enableDynamicPayloadLength();  //TODO: to tests
void disableDynamicPayloadLength(); //TODO: to tests

void enableAckPayload();  //TODO: to tests
void disableAckPayload(); //TODO: to tests

void enableDynamicAck(); //TODO: to tests
