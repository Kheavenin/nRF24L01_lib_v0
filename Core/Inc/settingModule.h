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

