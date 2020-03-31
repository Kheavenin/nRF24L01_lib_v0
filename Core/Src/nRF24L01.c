/**
 * @Autor: Khevenin
 * @Name: nRf24L01 library
 * @Version: v1.00 - first release
 *
 * Source file for nRF24L01+ library
 * Include functions.
 *
 * Before use read "ReadMe" file included to repo.
 */



#include "nRF24L01.h"

/* Static methods */
static void settingStruct_Init(nrfStruct_t *nrfStruct);
static void addressStruct_Init(nrfStruct_t *nrfStruct);
static void fifoStruct_Init(nrfStruct_t *nrfStruct);
static void statusStrcut_Init(nrfStruct_t *nrfStruct);
static void hardware_Init(nrfStruct_t *nrfStruct, SPI_HandleTypeDef *HAL_SPIx,
                          TIM_HandleTypeDef *HAL_TIMx, GPIO_TypeDef *HAL_GPIO_CSN,
                          uint16_t HAL_GPIO_Pin_CSN, GPIO_TypeDef *HAL_GPIO_CE,
                          uint16_t HAL_GPIO_Pin_CE);

static void statusStrcut_Init(nrfStruct_t *nrfStruct)
{
    nrfStruct->statusStruct.dataReadIrq = 0;
    nrfStruct->statusStruct.dataSendIrq = 0;
    nrfStruct->statusStruct.maxRetr = 0;
    nrfStruct->statusStruct.pipeNumber = RX_FIFO_EMPTY;
    nrfStruct->statusStruct.txFull = 0;

    nrfStruct->statusStruct.packetsLost = 0;
    nrfStruct->statusStruct.packetsRetr = 0;
}

static void settingStruct_Init(nrfStruct_t *nrfStruct)
{
    /* Init settigns struct */
    nrfStruct->setStruct.rxMode = 0;                //set as receiver
    nrfStruct->setStruct.channel = 0x02;            //set channel np. 0
    nrfStruct->setStruct.dataRate = RF_DataRate_2M; //lowest data rate
    nrfStruct->setStruct.powerRF = RF_PWR_0dBm;     //-12dBm power

    nrfStruct->setStruct.ard = 0; //auto retr. delay 250us
    nrfStruct->setStruct.arc = 3; //auto retr. counter

    nrfStruct->setStruct.enableCRC = 1;
    nrfStruct->setStruct.codingCRC = 0;

    nrfStruct->setStruct.enableTxIrq = 0;
    nrfStruct->setStruct.enableRxIrq = 0;
    nrfStruct->setStruct.enableMaxRtIrq = 0;

    /* Pipe Enable - defult pipe 0 enable only */
    nrfStruct->setStruct.pipeEn = DF_RXADDR;
    nrfStruct->setStruct.pipeACK = DF_EN_AA;
    nrfStruct->setStruct.pipeDPL = DF_DYNPD;

    /* Pipe RX Payload Lenght  */
    uint8_t i;
    for (i = 0; i < 6; i++)
    {
        nrfStruct->setStruct.pipePayLen[i] = DF_RX_PW_P0;
    }

    nrfStruct->setStruct.enableDPL = 0;
    nrfStruct->setStruct.enableAckPay = 0;
    nrfStruct->setStruct.enableDynACK = 0; //enable NO_ACK command
}

static void addressStruct_Init(nrfStruct_t *nrfStruct)
{
    /* Init address struct */
    nrfStruct->addrStruct.addrWidth = longWidth;
    uint8_t i;
    for (i = 0; i < 5; i++)
    {
        nrfStruct->addrStruct.txAddr[i] = DF_TX_ADDR_0;
    }
    for (i = 0; i < 5; ++i)
    {
        nrfStruct->addrStruct.rxAddr0[i] = DF_RX_ADDR_P0_0;
    }
    for (i = 0; i < 5; ++i)
    {
        nrfStruct->addrStruct.rxAddr1[i] = DF_RX_ADDR_P1_0;
    }
    nrfStruct->addrStruct.rxAddr2 = DF_RX_ADDR_P2;
    nrfStruct->addrStruct.rxAddr3 = DF_RX_ADDR_P3;
    nrfStruct->addrStruct.rxAddr4 = DF_RX_ADDR_P4;
    nrfStruct->addrStruct.rxAddr5 = DF_RX_ADDR_P5;
}

static void fifoStruct_Init(nrfStruct_t *nrfStruct)
{
    /* Init fifo struct */
    nrfStruct->fifoStruct.txReUse = 0;

    nrfStruct->fifoStruct.rxRead = 0;
    nrfStruct->fifoStruct.rxFull = 0;
    nrfStruct->fifoStruct.rxEmpty = 1;

    nrfStruct->fifoStruct.txSend = 0;
    nrfStruct->fifoStruct.txFull = 0;
    nrfStruct->fifoStruct.txEmpty = 1;
}

static void hardware_Init(nrfStruct_t *nrfStruct, SPI_HandleTypeDef *HAL_SPIx,
                          TIM_HandleTypeDef *HAL_TIMx, GPIO_TypeDef *HAL_GPIO_CSN,
                          uint16_t HAL_GPIO_Pin_CSN, GPIO_TypeDef *HAL_GPIO_CE,
                          uint16_t HAL_GPIO_Pin_CE)
{
    /* Put pointer of SPI and TIM structures to nRF alias */
    nrfStruct->nRFspi = HAL_SPIx;
    nrfStruct->nRFtim = HAL_TIMx;

    /* Put GPIO port pointer and pin number to structure's alias */
    nrfStruct->nRFportCSN = HAL_GPIO_CSN;
    nrfStruct->nRFpinCSN = HAL_GPIO_Pin_CSN;
    nrfStruct->nRFportCE = HAL_GPIO_CE;
    nrfStruct->nRFpinCE = HAL_GPIO_Pin_CE;
}

/* Micro sencods delay - necessary to SPI transmittion  */
void delayUs(nrfStruct_t *nrfStruct, uint16_t time)
{

    __HAL_TIM_SET_COUNTER((nrfStruct->nRFtim), 0); //Set star value as 0
    while (__HAL_TIM_GET_COUNTER(nrfStruct->nRFtim) < time)
        ;
}

/* CE snd CSN control funtions's */
void csnLow(nrfStruct_t *nrfStruct)
{
    HAL_GPIO_WritePin((nrfStruct->nRFportCSN), (nrfStruct->nRFpinCSN),
                      GPIO_PIN_RESET);
}
void csnHigh(nrfStruct_t *nrfStruct)
{
    HAL_GPIO_WritePin((nrfStruct->nRFportCSN), (nrfStruct->nRFpinCSN),
                      GPIO_PIN_SET);
}
void ceLow(nrfStruct_t *nrfStruct)
{
    HAL_GPIO_WritePin((nrfStruct->nRFportCE), (nrfStruct->nRFpinCE),
                      GPIO_PIN_RESET);
}
void ceHigh(nrfStruct_t *nrfStruct)
{
    HAL_GPIO_WritePin((nrfStruct->nRFportCE), (nrfStruct->nRFpinCE),
                      GPIO_PIN_SET);
}

/* Elementary functions base on nRf24L01+ SPI commands */
/* Read and write registers funtions's */
uint8_t readReg(nrfStruct_t *nrfStruct, uint8_t addr)
{
    uint8_t cmd = R_REGISTER | addr;
    uint8_t reg;
    uint8_t *pCmd = &cmd;
    uint8_t *pReg = &reg;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT);
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), pReg, sizeof(reg), SPI_TIMEOUT);

    csnHigh(nrfStruct);
    return reg;
}

void writeReg(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t val)
{
    uint8_t cmd = W_REGISTER | addr;
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT);
    delayUs(nrfStruct, 50);
    HAL_SPI_Transmit((nrfStruct->nRFspi), &val, sizeof(val), SPI_TIMEOUT);

    csnHigh(nrfStruct);
}

/* Extended read and write functions - R/W few registers */
void readRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
                size_t bufSize)
{
    uint8_t cmd = R_REGISTER | addr;
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT);
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), buf, bufSize,
                    SPI_TIMEOUT);

    csnHigh(nrfStruct);
}

void writeRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
                 size_t bufSize)
{
    uint8_t cmd = W_REGISTER | addr;
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT);
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), buf, bufSize,
                    SPI_TIMEOUT);

    csnHigh(nrfStruct);
}

/* Payload's functions */
uint8_t readRxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize)
{
    if (bufSize < 1)
        return ERR_CODE;
    if (bufSize > 32)
        bufSize = 32;

    uint8_t cmd = R_RX_PAYLOAD; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), buf, bufSize, SPI_TIMEOUT); //read payload

    csnHigh(nrfStruct);
    return OK_CODE;
}

uint8_t writeTxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize)
{
    if (bufSize < 1)
        return ERR_CODE;
    if (bufSize > 32)
        bufSize = 32;

    uint8_t cmd = W_TX_PAYLOAD; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    delayUs(nrfStruct, 50);
    HAL_SPI_Transmit((nrfStruct->nRFspi), buf, bufSize, SPI_TIMEOUT); //read payload

    csnHigh(nrfStruct);
    return OK_CODE;
}

uint8_t readDynamicPayloadWidth(nrfStruct_t *nrfStruct)
{
    uint8_t cmd = R_RX_PL_WID; //set command mask
    uint8_t width;
    uint8_t *pCmd = &cmd;
    uint8_t *pWidth = &width;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), pWidth, sizeof(width), SPI_TIMEOUT); //read payload

    csnHigh(nrfStruct);
    return width;
}

uint8_t writeTxPayloadAck(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize)
{
    if (bufSize < 1)
        return ERR_CODE;
    if (bufSize > 32)
        bufSize = 32;

    uint8_t cmd = W_ACK_PAYLOAD; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    delayUs(nrfStruct, 50);
    HAL_SPI_Transmit((nrfStruct->nRFspi), buf, bufSize, SPI_TIMEOUT); //read payload

    csnHigh(nrfStruct);
    return OK_CODE;
}

uint8_t writeTxPayloadNoAck(nrfStruct_t *nrfStruct, uint8_t *buf,
                            size_t bufSize)
{
    if (bufSize < 1)
        return ERR_CODE;
    if (bufSize > 32)
        bufSize = 32;

    uint8_t cmd = W_TX_PAYLOAD_NO_ACK; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    delayUs(nrfStruct, 50);
    HAL_SPI_Transmit((nrfStruct->nRFspi), buf, bufSize, SPI_TIMEOUT); //read payload

    csnHigh(nrfStruct);
    return OK_CODE;
}
/* Flush functions */
uint8_t flushTx(nrfStruct_t *nrfStruct)
{
    uint8_t cmd = FLUSH_TX; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    csnHigh(nrfStruct);

    if (!readBit(nrfStruct, FIFO_STATUS, bit4))
    { //check FIFO status
        nrfStruct->fifoStruct.txEmpty = 0;
        return ERR_CODE;
    }

    nrfStruct->fifoStruct.txEmpty = 1;
    return OK_CODE;
}
uint8_t flushRx(nrfStruct_t *nrfStruct)
{
    uint8_t cmd = FLUSH_RX; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    csnHigh(nrfStruct);

    if (!readBit(nrfStruct, FIFO_STATUS, bit0))
    { //check FIFO status
        nrfStruct->fifoStruct.rxEmpty = 0;
        return ERR_CODE;
    }

    nrfStruct->fifoStruct.rxEmpty = 1;
    return OK_CODE;
}

/* Others */
void reuseTxPayload(nrfStruct_t *nrfStruct)
{
    uint8_t cmd = REUSE_TX_PL; //set command mask
    uint8_t *pCmd = &cmd;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT); //send command
    csnHigh(nrfStruct);
}

uint8_t getStatus(nrfStruct_t *nrfStruct)
{
    uint8_t cmd = NOP;
    uint8_t *pCmd = &cmd;
    uint8_t reg = 0;
    uint8_t *pReg = &reg;

    csnLow(nrfStruct);

    HAL_SPI_Transmit((nrfStruct->nRFspi), pCmd, sizeof(cmd), SPI_TIMEOUT);
    delayUs(nrfStruct, 50);
    HAL_SPI_Receive((nrfStruct->nRFspi), pReg, sizeof(reg), SPI_TIMEOUT);

    csnHigh(nrfStruct);
    return reg;
}

/* Power control */
void pwrUp(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, CONFIG);
    tmp |= (1 << 1);
    writeReg(nrfStruct, CONFIG, tmp);
}
void pwrDown(nrfStruct_t *nrfStruct)
{
    ceLow(nrfStruct);
    uint8_t tmp = readReg(nrfStruct, CONFIG);
    tmp &= (0 << 1); //zmieniono OR na AND
    writeReg(nrfStruct, CONFIG, tmp);
}

uint8_t readBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit)
{
    uint8_t reg = readReg(nrfStruct, addr);
    return ((reg >> bit) & 0x01);
}

void resetBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit)
{
    uint8_t tmp = readReg(nrfStruct, addr);
    tmp &= ~(1 << bit); //zmieniono OR na AND
    writeReg(nrfStruct, addr, tmp);
}

void setBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit)
{
    uint8_t tmp = readReg(nrfStruct, addr);
    tmp |= (1 << bit);
    writeReg(nrfStruct, addr, tmp);
}

/* Main init function */
nrfStruct_t *nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
                      GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
                      GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE)
{

    /* Create main nRF structure	 */
    static nrfStruct_t nRFMainStruct;
    static nrfStruct_t *pnRFMainStruct = &nRFMainStruct;

    /* Init settigns struct */
    statusStrcut_Init(pnRFMainStruct);
    settingStruct_Init(pnRFMainStruct);
    addressStruct_Init(pnRFMainStruct);
    fifoStruct_Init(pnRFMainStruct);
    hardware_Init(pnRFMainStruct, HAL_SPIx, HAL_TIMx, HAL_GPIO_CSN,
                  HAL_GPIO_Pin_CSN, HAL_GPIO_CE, HAL_GPIO_Pin_CE);

    //	pwrDown(pnRFMainStruct);
    /* Turn on modul */
    pwrUp(pnRFMainStruct);

    /* Set default settings */
    setBit(pnRFMainStruct, CONFIG, PRIM_RX);
    writeReg(pnRFMainStruct, EN_AA, (pnRFMainStruct->setStruct.pipeACK));
    writeReg(pnRFMainStruct, EN_RXADDR, (pnRFMainStruct->setStruct.pipeEn));
    writeReg(pnRFMainStruct, SETUP_AW, DF_SETUP_AW);
    writeReg(pnRFMainStruct, SETUP_RETR, DF_SETUP_RETR);
    writeReg(pnRFMainStruct, RF_CH, DF_RF_CH);
    writeReg(pnRFMainStruct, RF_SETUP, DF_RF_SETUP);
    writeReg(pnRFMainStruct, STATUS, DF_STATUS);
    writeReg(pnRFMainStruct, DYNPD, (pnRFMainStruct->setStruct.pipeDPL));

    uint8_t i;
    for (i = 0; i < 6; i++)
    {
        writeReg(pnRFMainStruct, (RX_PW_P0 + i),
                 (pnRFMainStruct->setStruct.pipePayLen[i]));
    }
    writeReg(pnRFMainStruct, FEATURE, DF_FEATURE);

    /* Set default address */
    writeRegExt(pnRFMainStruct, TX_ADDR, (pnRFMainStruct->addrStruct.txAddr),
                sizeof(pnRFMainStruct->addrStruct.txAddr));
    writeRegExt(pnRFMainStruct, RX_ADDR_P0,
                (pnRFMainStruct->addrStruct.rxAddr0),
                sizeof(pnRFMainStruct->addrStruct.rxAddr0));
    writeRegExt(pnRFMainStruct, RX_ADDR_P1,
                (pnRFMainStruct->addrStruct.rxAddr1),
                sizeof(pnRFMainStruct->addrStruct.rxAddr1));
    writeReg(pnRFMainStruct, RX_ADDR_P2, (pnRFMainStruct->addrStruct.rxAddr2));
    writeReg(pnRFMainStruct, RX_ADDR_P3, (pnRFMainStruct->addrStruct.rxAddr3));
    writeReg(pnRFMainStruct, RX_ADDR_P4, (pnRFMainStruct->addrStruct.rxAddr4));
    writeReg(pnRFMainStruct, RX_ADDR_P5, (pnRFMainStruct->addrStruct.rxAddr5));

    return pnRFMainStruct;
}

uint8_t sendPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize)
{

    if (HAL_GPIO_ReadPin(CSN_GPIO_Port, CSN_Pin))
    {
        ceLow(nrfStruct);
    }
    if (getStatusFullTxFIFO(nrfStruct))
    {
        flushTx(nrfStruct);
    }
    if (getTX_DS(nrfStruct))
    {
        clearTX_DS(nrfStruct);
    }
    if (writeTxPayload(nrfStruct, buf, bufSize))
    {
        ceHigh(nrfStruct);
        delayUs(nrfStruct, CE_HIGH_TIME);
        ceLow(nrfStruct);
        if (getTX_DS(nrfStruct))
        {
            return OK_CODE;
        }
    }
    return 0;
}

uint8_t checkReceivedPayload(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (getPipeStatusRxFIFO(nrfStruct) == pipe)
    {
        if (getRX_DR(nrfStruct))
        {
            clearRX_DR(nrfStruct);
        }
        return 1;
    }
    return 0;
}

/* To turn off RX/TX state of module use mode Standby */
/**
 * @Brief	Switch radio module to Receiver (PRX) mode
 * @Retval	None
 */
void modeRX(nrfStruct_t *nrfStruct)
{
    if (!readBit(nrfStruct, CONFIG, bit1))
    { //Check state of module
        pwrUp(nrfStruct);
        delayUs(nrfStruct, 1500); //wait 1.5ms fo nRF24L01+ stand up
    }
    flushRx(nrfStruct); //clear (flush) RX FIFO buffer
    flushTx(nrfStruct); //clear (flush) TX FIFO buffer

    clearRX_DR(nrfStruct); //clear interrupts flags
    clearTX_DS(nrfStruct);
    clearMAX_RT(nrfStruct);
    //nRF in Standby-I
    ceHigh(nrfStruct); //set high on CE line
    setBit(nrfStruct, CONFIG, bit0);
    delayUs(nrfStruct, RX_TX_SETTING_TIME);
}

/**
 *@Brief	Switch radio module to Transmitter (PTX) mode
 */
void modeTX(nrfStruct_t *nrfStruct)
{
    if (!readBit(nrfStruct, CONFIG, bit1))
    { //Check state of module
        pwrUp(nrfStruct);
        delayUs(nrfStruct, 1500); //wait 1.5ms fo nRF24L01+ stand up
    }
    flushRx(nrfStruct); //clear (flush) RX FIFO buffer
    flushTx(nrfStruct); //clear (flush) TX FIFO buffer

    clearRX_DR(nrfStruct); //clear interrupts flags
    clearTX_DS(nrfStruct);
    clearMAX_RT(nrfStruct);

    ceHigh(nrfStruct);
    resetBit(nrfStruct, CONFIG, bit0);
    delayUs(nrfStruct, RX_TX_SETTING_TIME);
}

/**
 * @Brief	Switch radio module to Standby-I mode
 */
void modeStandby(nrfStruct_t *nrfStruct)
{
    ceLow(nrfStruct);
    resetBit(nrfStruct, CONFIG, bit0);
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
void clearIrqFlags(nrfStruct_t *nrfStruct)
{
    writeReg(nrfStruct, STATUS, 0x70);
    nrfStruct->statusStruct.dataReadIrq = 0;
    nrfStruct->statusStruct.dataSendIrq = 0;
    nrfStruct->statusStruct.maxRetr = 0;
}

uint8_t getRX_DR(nrfStruct_t *nrfStruct)
{
    nrfStruct->statusStruct.dataReadIrq = readBit(nrfStruct, STATUS, bit6);
    return (nrfStruct->statusStruct.dataReadIrq);
}

uint8_t getTX_DS(nrfStruct_t *nrfStruct)
{
    nrfStruct->statusStruct.dataSendIrq = readBit(nrfStruct, STATUS, bit5);
    return (nrfStruct->statusStruct.dataSendIrq);
}

uint8_t getMAX_RT(nrfStruct_t *nrfStruct)
{
    nrfStruct->statusStruct.maxRetr = readBit(nrfStruct, STATUS, bit4);
    return (nrfStruct->statusStruct.maxRetr);
}
/* CRC functions */
void enableCRC(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, CONFIG, bit3);
    nrfStruct->setStruct.enableCRC = 1;
}

void disableCRC(nrfStruct_t *nrfStruct)
{
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
uint8_t enableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (checkPipe(pipe))
    {
        setBit(nrfStruct, EN_RXADDR, pipe);
        nrfStruct->setStruct.pipeEn |= (1 << pipe);
        return 1;
    }
    return 0;
}
uint8_t disableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (checkPipe(pipe))
    {
        resetBit(nrfStruct, EN_RXADDR, pipe);
        nrfStruct->setStruct.pipeEn |= (0 << pipe);
        return 1;
    }
    return 0;
}

/* Address Width */
void setAddrWidth(nrfStruct_t *nrfStruct, addressWidth_t width)
{
    writeReg(nrfStruct, SETUP_AW, width);
    nrfStruct->addrStruct.addrWidth = width;
}

/* Setup retransmission */
uint8_t setAutoRetrCount(nrfStruct_t *nrfStruct, uint8_t count)
{
    if (count >= 0x00 && count <= 0x0F)
    {                                                 //check count val
        uint8_t tmp = readReg(nrfStruct, SETUP_RETR); //read reg. val
        tmp = tmp & 0xF0;                             // reset LSB and save MSB
        tmp |= count;                                 //add tmp and count
        writeReg(nrfStruct, SETUP_RETR, tmp);         //write to SETUP_RETR
        nrfStruct->setStruct.arc = count;
        return OK_CODE;
    }
    return ERR_CODE;
}

uint8_t setAutoRetrDelay(nrfStruct_t *nrfStruct, uint8_t delay)
{
    if (delay > 0x0F)
    {                       //if delay in MSB format
        delay = delay >> 4; //shift to LSB format
    }
    if (delay >= 0x00 && delay <= 0x0F)
    {
        uint8_t tmp = readReg(nrfStruct, SETUP_RETR);
        tmp = tmp & 0x0F;    //save LSB, reset MSB
        tmp |= (delay << 4); //add tmp and delay
        writeReg(nrfStruct, SETUP_RETR, tmp);
        nrfStruct->setStruct.ard = delay;
        return OK_CODE;
    }
    return ERR_CODE;
}

/* RF channel */
uint8_t setChannel(nrfStruct_t *nrfStruct, uint8_t channel)
{
    if (channel >= 0 && channel <= 125)
    {
        writeReg(nrfStruct, RF_CH, channel); //Maximum channel limited to 125 by hardware
        nrfStruct->setStruct.channel = channel;
        return OK_CODE;
    }
    return ERR_CODE;
}

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, RF_SETUP, bit7);
}
void disableContCarrier(nrfStruct_t *nrfStruct)
{
    resetBit(nrfStruct, RF_SETUP, bit7);
}
void enableLockPLL(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, RF_SETUP, bit4);
}
void diableLockPLL(nrfStruct_t *nrfStruct)
{
    resetBit(nrfStruct, RF_SETUP, bit4);
}
/* RPD - for test use only */
uint8_t checkRPD(nrfStruct_t *nrfStruct)
{
    if (readReg(nrfStruct, RPD))
        return 1;
    else
        return 0;
}

void setRFpower(nrfStruct_t *nrfStruct, powerRF_t power)
{
    /*
	if (power > RF_PWR_0dBm && power < RF_PWR_18dBm)
	 return ERR_CODE;*/
    uint8_t tmp = readReg(nrfStruct, RF_SETUP); //
    tmp = tmp & 0xF8;                           //0xF8 - 1111 1000B reset 3 LSB
    tmp = tmp | (power << 1);                   //combining tmp and shifted power
    writeReg(nrfStruct, RF_SETUP, tmp);
    nrfStruct->setStruct.powerRF = power;
}

void setDataRate(nrfStruct_t *nrfStruct, dataRate_t rate)
{
    uint8_t tmp = readReg(nrfStruct, RF_SETUP); //
    tmp = tmp & 0x06;                           //0x06 = 0000 0110B - reset data rate's bits - Also this line reset PLL_LOCK and CONT_WAVE bits
    tmp = tmp | (rate << 3);                    //combining tmp and shifted data rate
    writeReg(nrfStruct, RF_SETUP, tmp);
    nrfStruct->setStruct.dataRate = rate;
}

/* Status */
uint8_t getStatusFullTxFIFO(nrfStruct_t *nrfStruct)
{
    if (readBit(nrfStruct, STATUS, bit0))
    {
        nrfStruct->statusStruct.txFull = 1;
        return 1; //TX FIFO full
    }
    nrfStruct->statusStruct.txFull = 0;
    return 0; //Available locations in TX FIFO
}
/**
 * @Brief	Check pipe number with data to read 
 * */
uint8_t getPipeStatusRxFIFO(nrfStruct_t *nrfStruct)
{ //Zmieniono na kody bledow
    uint8_t tmp = readReg(nrfStruct, STATUS);
    tmp &= 0x0E; //save only pipe number bits
    tmp = tmp >> 1;
    if (checkPipe(tmp))
    {
        nrfStruct->statusStruct.pipeNumber = tmp;
        return tmp;
    }
    if (tmp == 0x07)
    { //RX FIFO empty
        nrfStruct->statusStruct.pipeNumber = RX_FIFO_EMPTY;
        return RX_FIFO_EMPTY;
    }

    if (tmp == 0x06)
    { //110B - mean not used
        nrfStruct->statusStruct.pipeNumber = RX_FIFO_UNUSED;
        return RX_FIFO_UNUSED; //return ERR
    }
    return ERR_CODE;
}

/* Transmit observe */
uint8_t lostPacketsCount(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, OBSERVE_TX);
    tmp = (tmp >> 4);
    nrfStruct->statusStruct.packetsLost = tmp;
    return tmp;
}

uint8_t retrPacketsCount(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, OBSERVE_TX);
    tmp = (tmp & 0xF0);
    nrfStruct->statusStruct.packetsRetr = tmp;
    return tmp;
}

void clearlostPacketsCount(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, RF_CH);
    writeReg(nrfStruct, RF_CH, tmp);
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
uint8_t setReceivePipeAddress(nrfStruct_t *nrfStruct, uint8_t pipe,
                              uint8_t *addrBuf, size_t addrBufSize)
{
    if (!checkPipe(pipe))
    { //if checkPipe return 0 - end fun. by return 0.
        return ERR_CODE;
    }
    size_t bufSize = 0x05;
    if (pipe == 0 || pipe == 1)
    { //if pipe 0 or 1 check bufer width
        switch (addrBufSize)
        { //check addrBufSize
        case 3:
            bufSize = 0x03;
            break;
        case 4:
            bufSize = 0x04;
            break;
        case 5:
            bufSize = 0x05;
            break;
        default:
            return ERR_CODE;
            break;
        }
        if (pipe == 0)
        { //check pipe and write addr to struct
            uint8_t i;
            for (i = 0; i < addrBufSize; i++)
            {
                nrfStruct->addrStruct.rxAddr0[i] = addrBuf[i];
            }
        }
        if (pipe == 1)
        {
            uint8_t i;
            for (i = 0; i < addrBufSize; i++)
            {
                nrfStruct->addrStruct.rxAddr1[i] = addrBuf[i];
            }
        }
    }
    else
    {
        if (addrBufSize == 1)
            bufSize = 0x01;
        switch (pipe)
        { //check pipe and write addr to struct
        case 2:
            nrfStruct->addrStruct.rxAddr2 = *addrBuf;
            break;
        case 3:
            nrfStruct->addrStruct.rxAddr3 = *addrBuf;
            break;
        case 4:
            nrfStruct->addrStruct.rxAddr4 = *addrBuf;
            break;
        case 5:
            nrfStruct->addrStruct.rxAddr5 = *addrBuf;
            break;
        default:
            return ERR_CODE;
            break;
        }
    }
    uint8_t addr = RX_ADDR_P0 + pipe; //if pipe = 0 -> write Receive address pipe 0
    writeRegExt(nrfStruct, addr, addrBuf, bufSize);

    return OK_CODE;
}

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(nrfStruct_t *nrfStruct, uint8_t *addrBuf,
                               size_t addrBufSize)
{
    if (((nrfStruct->addrStruct.addrWidth) + 2) != addrBufSize)
    {
        return ERR_CODE;
    }

    uint8_t i;
    for (i = 0; i < addrBufSize; i++)
    { //write to struct
        nrfStruct->addrStruct.txAddr[i] = addrBuf[i];
    }
    writeRegExt(nrfStruct, TX_ADDR, addrBuf, addrBufSize);
    return OK_CODE;
}

/* RX Payload width */
uint8_t getRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (checkPipe(pipe))
    {
        uint8_t addr = RX_PW_P0 + pipe;
        uint8_t tmp = readReg(nrfStruct, addr);
        nrfStruct->setStruct.pipePayLen[pipe] = tmp;
        return tmp;
    }
    return ERR_CODE;
}

uint8_t setRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe, uint8_t width)
{
    if (checkPipe(pipe))
    {
        if (width < 1 && width > 32)
        { //check width correct value
            return ERR_CODE;
        }
        uint8_t addr = RX_PW_P0 + pipe;
        writeReg(nrfStruct, addr, width);
        nrfStruct->setStruct.pipePayLen[pipe] = width;
        return OK_CODE;
    }
    return ERR_CODE;
}

/* FIFO status */
/**
 * @Brief	Return status of RX FIFO buffer by check bits in FIFO Status Register 
 * */
uint8_t getRxStatusFIFO(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, FIFO_STATUS);
    if ((tmp & 0x03) == RX_FIFO_MASK_EMPTY)
    {
        nrfStruct->fifoStruct.rxEmpty = 1;
        nrfStruct->fifoStruct.rxFull = 0;
        nrfStruct->fifoStruct.rxRead = 0;
        return RX_FIFO_MASK_EMPTY; //RX FIFO register buffer is empty
    }
    if ((tmp & 0x03) == RX_FIFO_MASK_FULL)
    {
        nrfStruct->fifoStruct.rxEmpty = 0;
        nrfStruct->fifoStruct.rxFull = 1;
        nrfStruct->fifoStruct.rxRead = 1;
        return RX_FIFO_MASK_FULL; ////RX FIFO register buffer is full
    }
    if ((tmp & 0x03) == RX_FIFO_MASK_DATA)
    {
        nrfStruct->fifoStruct.rxEmpty = 0;
        nrfStruct->fifoStruct.rxFull = 0;
        nrfStruct->fifoStruct.rxRead = 1;
        return RX_FIFO_MASK_DATA; //RX FIFO register buffer has some data but isn't full
    }
    return ERR_CODE;
}
/**
 * @Brief	Return status of TX FIFO buffer by check bits in FIFO Status Register 
 * */
uint8_t getTxStatusFIFO(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readReg(nrfStruct, FIFO_STATUS);
    tmp = tmp >> 4;
    if ((tmp & 0x03) == TX_FIFO_MASK_EMPTY)
    {
        nrfStruct->fifoStruct.txEmpty = 1;
        nrfStruct->fifoStruct.txFull = 0;
        nrfStruct->fifoStruct.txSend = 0;
        return TX_FIFO_MASK_EMPTY;
    }
    if ((tmp & 0x03) == TX_FIFO_MASK_FULL)
    {
        nrfStruct->fifoStruct.txEmpty = 0;
        nrfStruct->fifoStruct.txFull = 1;
        nrfStruct->fifoStruct.txSend = 1;
        return TX_FIFO_MASK_FULL;
    }
    if ((tmp & 0x03) == TX_FIFO_MASK_DATA)
    {
        nrfStruct->fifoStruct.txEmpty = 0;
        nrfStruct->fifoStruct.txFull = 0;
        nrfStruct->fifoStruct.txSend = 1;
        return TX_FIFO_MASK_DATA;
    }
    return ERR_CODE;
}
/**
 * @Brief	Checking reuse package
 * @Retval	TX_REUSE_USED mean that nRF24 module reuse to send again same package
 * 			TX_REUSE_UNUSED mena that nRF24 module doeasn't reuse to send again same package
 **/
uint8_t getTxReuse(nrfStruct_t *nrfStruct)
{
    uint8_t tmp = readBit(nrfStruct, FIFO_STATUS, TX_REUSE);
    nrfStruct->fifoStruct.txReUse = tmp;
    if (tmp == 0x01)
    {
        return TX_REUSE_USED;
    }
    return TX_REUSE_UNUSED;
}

/* Dynamic Payload Lenggth */
uint8_t enableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (!checkPipe(pipe))
    {
        return ERR_CODE;
    }
    setBit(nrfStruct, DYNPD, pipe);
    nrfStruct->setStruct.pipeDPL |= (1 << pipe);
    return OK_CODE;
}

uint8_t disableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe)
{
    if (!checkPipe(pipe))
    {
        return ERR_CODE;
    }
    resetBit(nrfStruct, DYNPD, pipe);
    nrfStruct->setStruct.pipeDPL |= (0 << pipe);
    return OK_CODE;
}
/* Feature */
void enableDynamicPayloadLength(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, FEATURE, EN_DPL);
    nrfStruct->setStruct.enableDPL = 1;
}
void disableDynamicPayloadLength(nrfStruct_t *nrfStruct)
{
    resetBit(nrfStruct, FEATURE, EN_DPL);
    nrfStruct->setStruct.enableDPL = 0;
}

void enableAckPayload(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, FEATURE, EN_ACK_PAY);
    nrfStruct->setStruct.enableAckPay = 1;
}
void disableAckPayload(nrfStruct_t *nrfStruct)
{
    resetBit(nrfStruct, FEATURE, EN_ACK_PAY);
    nrfStruct->setStruct.enableAckPay = 0;
}

/**
 * @Brief	Enable W_TX_PAYLOAD_NOACK command 
 * */
void enableNoAckCommand(nrfStruct_t *nrfStruct)
{
    setBit(nrfStruct, FEATURE, EN_DYN_ACK);
}
