#include "regCmd.h"
#include <main.h>
#include <stdint.h>
#include <stddef.h>

#define SPI_TIMEOUT 5

extern SPI_HandleTypeDef hspi2;

/* Elementary functions */
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);
/*
uint8_t getStatus();

void powerUp();
void powerDown();
void modeRX();
void modeTX();

uint8_t getStatus();
*/
