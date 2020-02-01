#include "regCmd.h"

#define SPI_TIMEOUT 5

/* Elementary functions */
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);

uint8_t getStatus();

/* Basic functions */
void powerUp();
void powerDown();
void modeRX();
void modeTX();

uint8_t getStatus();
