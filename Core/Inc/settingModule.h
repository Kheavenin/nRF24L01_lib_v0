
#include "highLevelModule.h"

uint8_t sendPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t checkReceivedPayload(nrfStruct_t *nrfStruct, uint8_t pipe);
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
void clearIrqFlags(nrfStruct_t *nrfStruct);

uint8_t getRX_DR(nrfStruct_t *nrfStruct);
uint8_t getTX_DS(nrfStruct_t *nrfStruct);
uint8_t getMAX_RT(nrfStruct_t *nrfStruct);


/* CRC */
void enableCRC(nrfStruct_t *nrfStruct);
void setCRC(nrfStruct_t *nrfStruct, widthCRC_t w);

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe);
uint8_t enableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);

/* RX addresses */
uint8_t enableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe);

/* Address Width */
void setAddrWidth(nrfStruct_t *nrfStruct, addressWidth_t width);

/* Setup retransmission */
uint8_t setAutoRetrCount(nrfStruct_t *nrfStruct, uint8_t count);
uint8_t setAutoRetrDelay(nrfStruct_t *nrfStruct, uint8_t delay);

/* RF channel */
uint8_t setChannel(nrfStruct_t *nrfStruct, uint8_t channel);

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier(nrfStruct_t *nrfStruct);
void disableContCarrier(nrfStruct_t *nrfStruct);
void enableLockPLL(nrfStruct_t *nrfStruct);
void diableLockPLL(nrfStruct_t *nrfStruct);

void setRFpower(nrfStruct_t *nrfStruct, powerRF_t power);
void setDataRate(nrfStruct_t *nrfStruct, dataRate_t rate);

/* Status */
uint8_t getStatusFullTxFIFO(nrfStruct_t *nrfStruct); //TODO: to tests
uint8_t getPipeStatusRxFIFO(nrfStruct_t *nrfStruct); //TODO: to tests

/* Transmit observe */
uint8_t lostPacketsCount(nrfStruct_t *nrfStruct); //TODO: to tests
uint8_t retrPacketsCount(nrfStruct_t *nrfStruct); //TODO: to tests
void clearlostPacketsCount(nrfStruct_t *nrfStruct);
/* RPD - for RF test use only */
uint8_t checkRPD(nrfStruct_t *nrfStruct);

/* Receive Address data pipe */
uint8_t setReceivePipeAddress(nrfStruct_t *nrfStruct, uint8_t pipe,
		uint8_t *addr, size_t addrBufSize); //TODO: to tests

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(nrfStruct_t *nrfStruct, uint8_t *addrBuf,
		size_t addrBufSize); //TODO: to tests

/* Payload width of each pipes */
uint8_t getRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe);//TODO: to tests
uint8_t setRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe, uint8_t width); //TODO: to tests

/* TX and RX FIFO */
uint8_t getRxStatusFIFO(nrfStruct_t *nrfStruct);
uint8_t getTxStatusFIFO(nrfStruct_t *nrfStruct);
uint8_t getTxReuse(nrfStruct_t *nrfStruct);       //TODO: to tests

/* DYNPD */
uint8_t enableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe); //TODO: to tests
uint8_t disableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe); //TODO: to tests

/* Feature */
void enableDynamicPayloadLength(nrfStruct_t *nrfStruct);  //TODO: to tests
void disableDynamicPayloadLength(nrfStruct_t *nrfStruct); //TODO: to tests

void enableAckPayload(nrfStruct_t *nrfStruct);  //TODO: to tests
void disableAckPayload(nrfStruct_t *nrfStruct); //TODO: to tests

void enableNoAckCommand(nrfStruct_t *nrfStruct); //TODO: to tests
