#include "regCmd.h"
#include <main.h>
#include <stdint.h>
#include <stddef.h>

#define SPI_TIMEOUT 10

extern SPI_HandleTypeDef hspi1;

/* CSN, CE lines drive functions */
void csnLow();	//TODO: in future create macro
void csnHigh();	//TODO: in future create macro
void ceLow();	//TODO: in future create macro
void ceHigh();	//TODO: in future create macro

/* Elementary functions */
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);


uint8_t readBit(uint8_t addr, uint8_t bit);	//TODO:  to refactoring
void setBit(uint8_t addr, uint8_t bit);
void resetBit(uint8_t addr, uint8_t bit);

void powerUp();		//TODO:  to refactoring
void powerDown();


/* Additional feature functions */
uint8_t getStatus();

/*
void modeRX();
void modeTX();


*/
