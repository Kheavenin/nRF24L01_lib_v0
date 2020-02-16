#include <main.h>
#include <stdint.h>
#include <stddef.h>

#include "regCmd.h"


#define SPI_TIMEOUT 10

extern SPI_HandleTypeDef hspi1;

/* CSN, CE lines drive functions */
void csnLow();	//TODO: in future create macro
void csnHigh();	//TODO: in future create macro
void ceLow();	//TODO: in future create macro
void ceHigh();	//TODO: in future create macro

/* Read/write register functions */
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);

/* Read/write single bit funtions */
uint8_t readBit(uint8_t addr, bitNum_t bit);
void setBit(uint8_t addr, bitNum_t bit);
void resetBit(uint8_t addr, bitNum_t bit);

/* Multi bytes read/write register functions */
void multiRead(uint8_t addr, uint8_t *buf, size_t bufSize);		//TODO: to test
void multiWrite(uint8_t addr, uint8_t *buf, size_t bufSize);	//TODO: to test

/* Turn on and turn off functions */
void powerUp();		//Fixed
void powerDown();




/* Additional feature functions */
uint8_t getStatus();

