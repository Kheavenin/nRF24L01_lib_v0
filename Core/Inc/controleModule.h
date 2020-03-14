#include <main.h>
#include <stdint.h>
#include <stddef.h>

#include "regCmd.h"

#define SPI_TIMEOUT 10

extern SPI_HandleTypeDef hspi1;
extern TIM_HandleTypeDef htim1;



/* CSN, CE lines drive functions */
void csnLow();  //TODO: in future create macro
void csnHigh(); //TODO: in future create macro
void ceLow();   //TODO: in future create macro
void ceHigh();  //TODO: in future create macro



/* Read/write register functions */
uint8_t readRegister(uint8_t addr);
void writeRegister(uint8_t addr, uint8_t val);

/* Multi bytes read/write register functions */
void multiRead(uint8_t addr, uint8_t *buf, size_t bufSize);
void multiWrite(uint8_t addr, uint8_t *buf, size_t bufSize);

/*
 * TX and RX payloads access functions
 */

/*
 * @Brief	Read received payload
 * @Param	Pointer to payload's buffer
 * @Param	Buffer size
 * @Retval	Return status of execution
 * 			OK_CODE if arguments are correct or function execute correct
 * 			ERR_CODE if arguments are incorrect or function return error of execution
 */
uint8_t readRxPayload(uint8_t *buf, size_t bufSize);

/*
 * @Brief	Write  payload to transmit
 * @Param	Pointer with payload's buffer
 * @Param	Buffer size
 * @Retval	Return status of execution
 * 			OK_CODE if arguments are correct or function execute correct
 * 			ERR_CODE if arguments are incorrect or function return error of execution
 */
uint8_t writeTxPayload(uint8_t *buf, size_t bufSize);

/*
 * @Brief	Read received payload width for the top
 * @Param	Pointer to payload's buffer
 * @Param	Buffer size
 * @Param 	Width of bytes to read
 * @Retval	Return status of execution
 * 			OK_CODE if arguments are correct or function execute correct
 * 			ERR_CODE if arguments are incorrect or function return error of execution
 */
uint8_t readRxPayloadWidth(uint8_t *buf, size_t bufSize, uint8_t width);

/*
 * @Brief	Write  payload to transmit together with ACK packet
 * @Param	Pointer with payload's buffer
 * @Param	Buffer size
 * @Retval	Return status of execution
 * 			OK_CODE if arguments are correct or function execute correct
 * 			ERR_CODE if arguments are incorrect or function return error of execution
 */
uint8_t writeTxPayloadAck(uint8_t *buf, size_t bufSize);

/*
 * @Brief	Write payload to transmit without ACK packet on this specify
 * @Param	Pointer with payload's buffer
 * @Param	Buffer size
 * @Retval	Return status of execution
 * 			OK_CODE if arguments are correct or function execute correct
 * 			ERR_CODE if arguments are incorrect or function return error of execution
 */
uint8_t writeTxPayloadNoAck(uint8_t *buf, size_t bufSize);

/*
 * @Brieg	Flush TX FIFO
 * @Retval	OK_CODE if flush will be execute
 * 			ERR_CODE if flush will not be execute
 */
uint8_t flushTx();

/*
 * @Brieg	Flush RX FIFO
 * @Retval	OK_CODE if flush will be execute
 * 			ERR_CODE if flush will not be execute
 */
uint8_t flushRx();

/*
 * @Brief	Reuse last transmitted payload
 */
void reuseTxPayload();

/* Additional feature functions */
uint8_t getStatus();

/* Turn on and turn off module */
void powerUp();
void powerDown();


/*
 * Important
 * Delay function to make gap between SPI command and data.
 * Create delay_us function which use timer.
 * This function enable micro seconds delay.
 * Also HAL_Delay can be use, but changes in code are necessary.
 */
void DelayUs(uint16_t time);





















