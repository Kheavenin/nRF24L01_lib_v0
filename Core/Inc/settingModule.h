#include <main.h>
#include <stdint.h>
#include <stddef.h>

#include "controleModule.h"

/* Modes */
void modeRX();
void modeTX();
void modeStandby();

/* Interrupts */
void disableTXinterrupt();	//mask data send interrupt
void disableRXinterrupt();	//mask data receive interrupt
void disableRTinterrupt();	//mask data retransmition interrupt

void enableTXinterrupt();	//dont mask data send interrupt
void enableRXinterrupt();	//dont mask data receive interrupt
void enableRTinterrupt();	//dont mask data retransmition interrupt

void clearRX_DR();		//clear irt bits in Status Register
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
