#include "settingModule.h"


//	nrfStruct_t *nrfStruct

/**
 * @Brief	Switch radio module to Receiver (PRX) mode
 * @Retval	None
 */
void modeRX(nrfStruct_t *nrfStruct)
{
	ceHigh(nrfStruct); //set high on CE line
	setBit(CONFIG, bit0);
}

/**
 *@Brief	Switch radio module to Transmitter (PTX) mode
 */
void modeTX(nrfStruct_t *nrfStruct)
{
	ceHigh(nrfStruct);
	resetBit(CONFIG, bit0);
}

/**
 * @Brief	Switch radio module to Standby-I mode
 */
void modeStandby(nrfStruct_t *nrfStruct)
{
	ceLow(nrfStruct);
}

/* Interrupts functions */
void disableRXinterrupt(nrfStruct_t *nrfStruct)
{
	resetBit(nrfStruct, CONFIG, bit6); //disable RX_IRQ in Config register
	nrfStruct->setStruct.enableRxIrq = 0;
}
void disableTXinterrupt(nrfStruct_t *nrfStruct)
{
	resetBit(nrfStruct, CONFIG, bit5);
	nrfStruct->setStruct.enableTxIrq = 0;
}
void disableMaxRTinterrupt(nrfStruct_t *nrfStruct)
{
	resetBit(nrfStruct, CONFIG, bit4);
	nrfStruct->setStruct.enableMaxRtIrq = 0;
}

void enableRXinterrupt(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, CONFIG, bit6);
	nrfStruct->setStruct.enableRxIrq = 1;
}
void enableTXinterrupt(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, CONFIG, bit5);
	nrfStruct->setStruct.enableTxIrq = 1;
}
void enableRTinterrupt(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, CONFIG, bit4);
	nrfStruct->setStruct.enableMaxRtIrq = 1;
}

void clearRX_DR(nrfStruct_t *nrfStruct)
{ //clear irt bits in Status Register
	setBit(nrfStruct, STATUS, bit6);
	nrfStruct->statusStruct.dataReadIrq = 0;
}
void clearTX_DS(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, STATUS, bit5);
	nrfStruct->statusStruct.dataSendIrq = 0;
}
void clearMAX_RT(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, STATUS, bit4);
	nrfStruct->statusStruct.maxRetr = 0;
}

/* CRC functions */
void enableCRC(nrfStruct_t *nrfStruct)
{
	setBit(nrfStruct, CONFIG, bit3);
	nrfStruct->setStruct.enableCRC = 1;
}

void disableCRC(nrfStruct_t *nrfStruct) {
	resetBit(nrfStruct, CONFIG, bit3);
	nrfStruct->setStruct.enableCRC = 0;
}

void setCRC(nrfStruct_t *nrfStruct, widthCRC_t w)
{
	if (w)
	{
		setBit(nrfStruct, CONFIG, bit2);
		nrfStruct->setStruct.codingCRC = 1;
	}
	else
	{
		resetBit(nrfStruct, CONFIG, bit2);
		nrfStruct->setStruct.codingCRC = 0;
	}
}

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe)
{
	if (pipe >= 0 && pipe <= 5) //check correct pipe number
		return 1;
	return 0;
}
uint8_t enableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe)
{
	if (checkPipe(pipe))
	{
		setBit(nrfStruct, EN_AA, pipe);
		nrfStruct->setStruct.pipeACK |= (1 << pipe);
		return 1;
	}
	return 0;
}
uint8_t disableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe)
{
	if (checkPipe(pipe))
	{
		resetBit(nrfStruct, EN_AA, pipe);
		nrfStruct->setStruct.pipeACK |= 0 << pipe;
		return 1;
	}
	return 0;
}

/* RX addresses */
uint8_t enableRxAddr(uint8_t pipe)
{
	if (checkPipe(pipe))
	{
		setBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}
uint8_t disableRxAddr(uint8_t pipe)
{
	if (checkPipe(pipe))
	{
		resetBit(EN_AA, pipe);
		return 1;
	}
	return 0;
}

/* Address Width */
void setAddrWidth(addressWidth_t width)
{
	writeRegister(SETUP_AW, width);
}

/* Setup retransmission */
uint8_t setAutoRetrCount(uint8_t count)
{
	if (count >= 0x00 && count <= 0x0F)
	{											//check count val
		uint8_t tmp = readRegister(SETUP_RETR); //read reg. val
		tmp = tmp & 0xF0;						// reset LSB and save MSB
		tmp |= count;							//add tmp and count
		writeRegister(SETUP_RETR, tmp);			//write to SETUP_RETR
		return OK_CODE;
	}
	return ERR_CODE;
}

uint8_t setAutoRetrDelay(uint8_t delay)
{
	if (delay > 0x0F)
	{						//if delay in MSB format
		delay = delay >> 8; //shift to LSB format
	}
	if (delay >= 0x00 && delay <= 0x0F)
	{
		uint8_t tmp = readRegister(SETUP_RETR);
		tmp = tmp & 0x0F;	//save LSB, reset MSB
		tmp |= (delay << 8); //add tmp and delay
		writeRegister(SETUP_RETR, tmp);
		return OK_CODE;
	}
	return ERR_CODE;
}

/* RF channel */
uint8_t setChannel(uint8_t channel)
{
	if (channel >= 0 && channel <= 125)
	{
		writeRegister(RF_CH, channel); //Maximum channel limited to 125 by hardware
		return OK_CODE;
	}
	return ERR_CODE;
}

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier()
{
	setBit(RF_SETUP, bit7);
}
void disableContCarrier()
{
	resetBit(RF_SETUP, bit7);
}
void enableLockPLL()
{
	setBit(RF_SETUP, bit4);
}
void diableLockPLL()
{
	resetBit(RF_SETUP, bit4);
}

void setRFpower(powerRF_t power)
{
	/*
	if (power > RF_PWR_0dBm && power < RF_PWR_18dBm)
	 return ERR_CODE;*/
	uint8_t tmp = readRegister(RF_SETUP); //
	tmp = tmp & 0xF8;					  //0xF8 - 1111 1000B reset 3 LSB
	tmp = tmp | (power << 1);			  //combining tmp and shifted power
	writeRegister(RF_SETUP, tmp);
	/* return OK_CODE; */
}

void setDataRate(dataRate_t rate)
{
	uint8_t tmp = readRegister(RF_SETUP); 	//
	tmp = tmp & 0x06;//0x06 = 0000 0110B - reset data rate's bits - Also this line reset PLL_LOCK and CONT_WAVE bits
	tmp = tmp | (rate << 3);			  //combining tmp and shifted data rate
	writeRegister(RF_SETUP, tmp);
}

/* Status */
/**
 * @Brief check fill of TX FIFO 
 * */
uint8_t getStatusFullTxFIFO()
{
	if (readBit(STATUS, bit0))
		return 0; //TX FIFO full
	return 1;	 //Available locations in TX FIFO
}
/**
 * @Brief	Check pipe number with data to read 
 * */
uint8_t getPipeStatusRxFIFO()
{ //Zmieniono na kody bledow
	uint8_t tmp = readRegister(STATUS);
	tmp &= 0x0E; //save only pipe number bits
	tmp = tmp >> 1;
	if (checkPipe(tmp))
		return tmp;
	if (tmp == 0x07) //RX FIFO empty
		return RX_FIFO_EMPTY;
	if (tmp == 0x06)		   //110B - mean not used
		return RX_FIFO_UNUSED; //return ERR
	return ERR_CODE;
}

/* Transmit observe */
uint8_t lostPacketsCount()
{
	uint8_t tmp = readRegister(OBSERVE_TX);
	return (tmp >> 4);
}

uint8_t retrPacketsCount()
{
	uint8_t tmp = readRegister(OBSERVE_TX);
	return (tmp & 0xF0);
}

/* RPD - for test use only */
uint8_t checkRPD()
{
	if (readRegister(RPD))
		return 1;
	else
		return 0;
}

/* Receive Address data pipe */
/**
 * @Brief	Write receiver address of pipe
 * @Param	Number of pipe
 * @Param	Pointer to buffer with address
 * @Param	Size of address's buffer
 * @Note	Remember that addresses registers for pipes from 2 to 5 are 1 byte only.
 * 			Also registers for pipe 0 and 1 can have size of from 3 to 5 bytes.
 */
uint8_t setReceivePipeAddress(uint8_t pipe, uint8_t *addrBuf,
							  size_t addrBufSize)
{
	if (!checkPipe(pipe))
	{ //if checkPipe return 0 - end fun. by return 0.
		return ERR_CODE;
	}
	uint8_t addr = RX_ADDR_P0 + pipe; //if pipe = 0 -> write Receive address pipe 0
	if (pipe >= 2 && pipe <= 5)
	{
		if (addrBufSize != 1)
		{
			return ERR_CODE;
		}
	}
	multiWrite(addr, addrBuf, addrBufSize);
	return OK_CODE;
}

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(uint8_t *addrBuf, size_t addrBufSize)
{
	if (addrBufSize != 5)
	{
		return ERR_CODE; //if addrBufSize isn't 5 bytes retun 0
	}
	multiWrite(TX_ADDR, addrBuf, addrBufSize);
	return OK_CODE;
}

/* RX Payload width */
uint8_t getRxPayloadWidth(uint8_t pipe) {
	if (checkPipe(pipe))
	{
		uint8_t addr = RX_PW_P0 + pipe;
		return readRegister(addr);
	}
  return ERR_CODE;
}

uint8_t setRxPayload(uint8_t pipe, uint8_t width)
{
	if (checkPipe(pipe))
	{
		if (width < 1 && width > 32)
		{ //check width correct value
	  return ERR_CODE;
		}
      uint8_t addr = RX_PW_P0 + pipe;
		writeRegister(addr, width);
		return 1;
	}
  return ERR_CODE;
}

/* FIFO status */
/**
 * @Brief	Return status of RX FIFO buffer by check bits in FIFO Status Register 
 * */
uint8_t getRxStatusFIFO()
{
	uint8_t tmp = readRegister(FIFO_STATUS);
	if ((tmp & 0x03) == RX_FIFO_MASK_EMPTY)
	{
		return RX_FIFO_MASK_EMPTY; //RX FIFO register buffer is empty
	}
	if ((tmp & 0x03) == RX_FIFO_MASK_FULL)
	{
		return RX_FIFO_MASK_FULL; ////RX FIFO register buffer is full
	}
	if ((tmp & 0x03) == RX_FIFO_MASK_DATA)
	{
		return RX_FIFO_MASK_DATA; //RX FIFO register buffer has some data but isn't full
	}
	return ERR_CODE;
}
/**
 * @Brief	Return status of TX FIFO buffer by check bits in FIFO Status Register 
 * */
uint8_t getTxStatusFIFO()
{
	uint8_t tmp = readRegister(FIFO_STATUS);
	tmp = tmp >> 4;
	if ((tmp & 0x03) == TX_FIFO_MASK_EMPTY)
	{
		return TX_FIFO_MASK_EMPTY;
	}
	if ((tmp & 0x03) == TX_FIFO_MASK_FULL)
	{
		return TX_FIFO_MASK_FULL;
	}
	if ((tmp & 0x03) == TX_FIFO_MASK_DATA)
	{
		return TX_FIFO_MASK_DATA;
	}
	return ERR_CODE;
}
/**
 * @Brief	Checking reuse package
 * @Retval	TX_REUSE_USED mean that nRF24 module reuse to send again same package
 * 			TX_REUSE_UNUSED mena that nRF24 module doeasn't reuse to send again same package
 **/
uint8_t getTxReuse()
{
	uint8_t tmp = readBit(FIFO_STATUS, TX_REUSE);
	if (tmp == 0x01)
	{
		return TX_REUSE_USED;
	}
	return TX_REUSE_UNUSED;
}

/* Dynamic Payload Lenggth */
uint8_t enableDynamicPayloadLengthPipe(uint8_t pipe)
{
	if (!checkPipe(pipe))
	{
		return ERR_CODE;
	}
	setBit(DYNPD, pipe);
	return OK_CODE;
}

uint8_t disableDynamicPayloadLengthPipe(uint8_t pipe)
{
	if (!checkPipe(pipe))
	{
		return ERR_CODE;
	}
	resetBit(DYNPD, pipe);
	return OK_CODE;
}
/* Feature */
void enableDynamicPayloadLength()
{
	setBit(FEATURE, EN_DPL);
}
void disableDynamicPayloadLength()
{
	resetBit(FEATURE, EN_DPL);
}

void enableAckPayload()
{
	setBit(FEATURE, EN_ACK_PAY);
}
void disableAckPayload()
{
  resetBit (FEATURE, EN_ACK_PAY);
}

/**
 * @Brief	Enable W_TX_PAYLOAD_NOACK command 
 * */
void enableDynamicAck()
{
	setBit(FEATURE, EN_DYN_ACK);
}
