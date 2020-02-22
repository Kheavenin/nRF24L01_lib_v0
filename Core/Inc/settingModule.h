#include <main.h>
#include <stdint.h>
#include <stddef.h>

#include "controleModule.h"

/* Modes */
void modeRX();
void modeTX();
void modeStandby();

/* Interrupts */
void disableTXinterrupt(); //mask data send interrupt
void disableRXinterrupt(); //mask data receive interrupt
void disableRTinterrupt(); //mask data retransmition interrupt

void enableTXinterrupt(); //dont mask data send interrupt
void enableRXinterrupt(); //dont mask data receive interrupt
void enableRTinterrupt(); //dont mask data retransmition interrupt

void clearRX_DR(); //clear irt bits in Status Register
void clearTX_DS();
void clearMAX_RT();

/* CRC */
void enableCRC();
void setCRC(widthCRC_t w);

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe);
uint8_t enableAutoAckPipe(uint8_t pipe);
uint8_t disableAutoAckPipe(uint8_t pipe);

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
