/**
 * @Autor: Khevenin
 * Structures, init structures 
 * Register list of nRF24L01+ and deafult value of registers.
 * All addresses are 8 bits.
*/

#include <stdint.h>
#include <stddef.h>

/**
 * Map rregisters
 **/

/** Control registers */
#define CONFIG 0x00     //PWR, role, interrupts
#define EN_AA 0x01      //enable auto ack
#define EN_RXADDR 0x02  //enable RX addresses
#define SETUP_AW 0x03   //setup of address widths
#define SETUP_RETR 0x04 //setup of automatic retrasmission
#define RF_CH 0x05      //RF channel
#define RF_SETUP 0x06   //RF setup register
#define STATUS 0x07     //Status register
#define OBSERVE_TX 0x08 //Transmit observe register
#define RPD 0x09        //Received Power Detector

/** Address registers */
#define RX_ADDR_P0 0x0A //Received address data pipe 0
#define RX_ADDR_P1 0x0B //Received address data pipe 1
#define RX_ADDR_P2 0x0C //Received address data pipe 2
#define RX_ADDR_P3 0x0D //Received address data pipe 3
#define RX_ADDR_P4 0x0E //Received address data pipe 4
#define RX_ADDR_P5 0x0F //Received address data pipe 5

#define TX_ADDR 0x10 //Transmit addresses

/** Size of payload */
#define RX_PW_P0 0x11 //Number of bytes in RX payload in data pipe 0 - max 32 bytes
#define RX_PW_P1 0x12 //Number of bytes in RX payload in data pipe 1 - max 32 bytes
#define RX_PW_P2 0x13 //Number of bytes in RX payload in data pipe 2 - max 32 bytes
#define RX_PW_P3 0x14 //Number of bytes in RX payload in data pipe 3 - max 32 bytes
#define RX_PW_P4 0x15 //Number of bytes in RX payload in data pipe 4 - max 32 bytes
#define RX_PW_P5 0x16 //Number of bytes in RX payload in data pipe 5 - max 32 bytes

/** Another feature */
#define FIFO_STATUS 0x17 //FIFO status register
#define DYNPD 0x1C       //Enable dynamic payload lenght
#define FEATURE 0x1D     // Feature register

/**
 * Deafult values of registers
 **/
#define DF_CONFIG 0x08
#define DF_EN_AA 0x3F //DEX - 63
#define DF_RXADDR 0x03
#define DF_SETUP_AW 0x03
#define DF_SETUP_RETR 0x03
#define DF_RF_CH 0x02
#define DF_RF_SETUP 0x0E
#define DF_STATUS 0x0E     // B - 3:0 - read only
#define DF_OBSERVE_TX 0x00 //read only
#define DF_RPD 0x00        //read only

#define DF_RX_ADDR_P0_0 0xE7
#define DF_RX_ADDR_P0_1 0xE7
#define DF_RX_ADDR_P0_2 0xE7
#define DF_RX_ADDR_P0_3 0xE7
#define DF_RX_ADDR_P0_4 0xE7

#define DF_RX_ADDR_P1_0 0xC2
#define DF_RX_ADDR_P1_1 0xC2
#define DF_RX_ADDR_P1_2 0xC2
#define DF_RX_ADDR_P1_3 0xC2
#define DF_RX_ADDR_P1_4 0xC2

#define DF_RX_ADDR_P2 0xC3
#define DF_RX_ADDR_P3 0xC4
#define DF_RX_ADDR_P4 0xC5
#define DF_RX_ADDR_P5 0xC6

#define DF_TX_ADDR_0 0xE7
#define DF_TX_ADDR_1 0xE7
#define DF_TX_ADDR_2 0xE7
#define DF_TX_ADDR_3 0xE7
#define DF_TX_ADDR_4 0xE7

#define DF_RX_PW_P0 0x00
#define DF_RX_PW_P1 0x00
#define DF_RX_PW_P2 0x00
#define DF_RX_PW_P3 0x00
#define DF_RX_PW_P4 0x00
#define DF_RX_PW_P5 0x00

#define DF_FIFO_STATUS 0x11 // B - 6:4, 1:0 - read only
#define DF_DYNPD 0x00
#define DF_FEATURE 0x00

/* Config register */
#define MASK_RX_DR bit6
#define MASK_TX_DS bit5
#define MASK_MAX_RT bit4
#define EN_CRC bit3
#define CRCO bit2
#define PWR_UP bit1
#define PRIM_RX bit0

/* RF setup register */
#define CONT_WAVE bit7
#define RF_DR_LOW bit5
#define PLL_LOCK bit4
#define RF_DR_HIGH bit3
#define RF_PWR_HIGH bit2
#define RF_PWR_LOW bit1

/* Status register */
#define RX_DR bit6  //Data read interrupt
#define TX_DS bit5  //Data send interrupt
#define MAX_RT bit4 //Max number of retransmits interrupt
#define TX_FULL bit0

/* Received Power Detector register */
#define RPD_FLAG bit0

/* FIFO status register */
#define TX_REUSE bit6
#define TX_FULL_FIFO bit5
#define TX_EMPTY bit4
#define RX_FULL bit1
#define RX_EMPTY bit0

/* Dynamic Payload register */
#define DPL_P5 bit5
#define DPL_P4 bit4
#define DPL_P3 bit3
#define DPL_P2 bit2
#define DPL_P1 bit1
#define DPL_P0 bit0

/* Feature register */
#define EN_DPL bit2
#define EN_ACK_PAY bit1
#define EN_DYN_ACK bit0

/**
 * nRF24L01+ internal commands
 */
#define R_REGISTER 0x00 //000a aaaa
#define W_REGISTER 0x20 //001a aaaa
#define R_RX_PAYLOAD 0x61
#define W_TX_PAYLOAD 0xA0
#define FLUSH_TX 0xE1
#define FLUSH_RX 0xE0
#define REUSE_TX_PL 0xE0
#define R_RX_PL_WID 0x60
#define W_ACK_PAYLOAD 0xA8 //1010 1ppp
#define W_TX_PAYLOAD_NO_ACK 0xB0
#define NOP 0xFF

/* Bit definitions */
typedef enum
{
    bit7 = 7,
    bit6 = 6,
    bit5 = 5,
    bit4 = 4,
    bit3 = 3,
    bit2 = 2,
    bit1 = 1,
    bit0 = 0
} bitNum_t;

/* power enum typedef */
typedef enum
{
    RF_PWR_0dBm = 0x03,
    RF_PWR_6dBm = 0x02,
    RF_PWR_12dBm = 0x01,
    RF_PWR_18dBm = 0x00
} powerRF_t;

/* data rate enum typedef */
typedef enum
{
    RF_DataRate_250 = 0x04,
    RF_DataRate_1M = 0x00,
    RF_DataRate_2M = 0x01
} dataRate_t;

/* CRC coding */
typedef enum
{
    CRC_8_bits = 0,
    CRC_16_bits = 1
} widthCRC_t;

/* address width typedef */
typedef enum
{
    shortWidth = 0x01,
    mediumWidth = 0x02,
    longWidth = 0x03
} addressWidth_t;

/*
 * Functions return codes
 */
#define OK_CODE 0x01
#define ERR_CODE 0xFF

#define RX_FIFO_EMPTY 0x07
#define RX_FIFO_UNUSED 0x06

#define RX_FIFO_MASK_DATA 0x00
#define RX_FIFO_MASK_EMPTY 0x01
#define RX_FIFO_MASK_FULL 0x02

#define TX_FIFO_MASK_DATA 0x00
#define TX_FIFO_MASK_EMPTY 0x01
#define TX_FIFO_MASK_FULL 0x02

#define TX_REUSE_UNUSED 0x00
#define TX_REUSE_USED 0x01

#define SPI_TIMEOUT 10
#define CE_HIGH_TIME 11
#define RX_TX_SETTING_TIME 140

/* Structures */
typedef struct
{
    uint8_t dataReadIrq : 1;
    uint8_t dataSendIrq : 1;
    uint8_t maxRetr : 1;
    uint8_t pipeNumber : 3;
    uint8_t txFull : 1;

    uint8_t packetsLost;
    uint8_t packetsRetr;
} statusStruct_t;

typedef struct
{
    uint8_t rxMode : 1;
    uint8_t channel;
    dataRate_t dataRate;
    powerRF_t powerRF;

    uint8_t ard; //automatic retransmissions
    uint8_t arc; //auto retransmit count

    uint8_t pipeEn;
    uint8_t pipeACK;
    uint8_t pipeDPL;
    uint8_t pipePayLen[6];

    uint8_t enableCRC : 1;
    uint8_t codingCRC : 1;

    uint8_t enableTxIrq : 1;
    uint8_t enableRxIrq : 1;
    uint8_t enableMaxRtIrq : 1;

    uint8_t enableDPL : 1;
    uint8_t enableAckPay : 1;
    uint8_t enableDynACK : 1;
} settingsStruct_t;

typedef struct
{
    addressWidth_t addrWidth;
    uint8_t txAddr[5]; //5 byte register

    uint8_t rxAddr0[5]; //5 byte register
    uint8_t rxAddr1[5]; //5 byte register
    uint8_t rxAddr2;    //1 byte registers
    uint8_t rxAddr3;
    uint8_t rxAddr4;
    uint8_t rxAddr5;
} addrresStruct_t;

typedef struct
{
    uint8_t txReUse : 1;

    uint8_t rxRead : 1;
    uint8_t rxFull : 1;
    uint8_t rxEmpty : 1;

    uint8_t txSend : 1;
    uint8_t txFull : 1;
    uint8_t txEmpty : 1;
} fifoStruct_t;

typedef struct
{
    settingsStruct_t setStruct;
    addrresStruct_t addrStruct;
    fifoStruct_t fifoStruct;
    statusStruct_t statusStruct;

    SPI_HandleTypeDef *nRFspi;
    TIM_HandleTypeDef *nRFtim;

    GPIO_TypeDef *nRFportCSN;
    uint16_t nRFpinCSN;
    GPIO_TypeDef *nRFportCE;
    uint16_t nRFpinCE;
} nrfStruct_t;

/* Main init functions */
nrfStruct_t *nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
                      GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
                      GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE);

/* Delay for SPI  */
void delayUs(nrfStruct_t *nrfStruct, uint16_t time);

/* CE snd CSN control funtions's */
void csnLow(nrfStruct_t *nrfStruct);  //TODO: in future create macro
void csnHigh(nrfStruct_t *nrfStruct); //TODO: in future create macro
void ceLow(nrfStruct_t *nrfStruct);   //TODO: in future create macro
void ceHigh(nrfStruct_t *nrfStruct);  //TODO: in future create macro

/* Elementary functions base on nRf24L01+ SPI commands */
/* Read and write registers funtions's */
uint8_t readReg(nrfStruct_t *nrfStruct, uint8_t addr);
void writeReg(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t val);
/* Extended read and write functions - R/W few registers */
void readRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
                size_t bufSize);
void writeRegExt(nrfStruct_t *nrfStruct, uint8_t addr, uint8_t *buf,
                 size_t bufSize);

/* Payload's functions */
uint8_t readRxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayloadAck(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t writeTxPayloadNoAck(nrfStruct_t *nrfStruct, uint8_t *buf,
                            size_t bufSize);

/* Read payload lenght when DPL enable */
uint8_t readDynamicPayloadWidth(nrfStruct_t *nrfStruct);

/* Flush functions */
uint8_t flushTx(nrfStruct_t *nrfStruct);
uint8_t flushRx(nrfStruct_t *nrfStruct);

/* Others */
void reuseTxPayload(nrfStruct_t *nrfStruct);
uint8_t getStatus(nrfStruct_t *nrfStruct);

/* Bit's functions */
uint8_t readBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);
void resetBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);
void setBit(nrfStruct_t *nrfStruct, uint8_t addr, bitNum_t bit);

/* Power control */
void pwrUp(nrfStruct_t *nrfStruct);
void pwrDown(nrfStruct_t *nrfStruct);

uint8_t sendPayload(nrfStruct_t *nrfStruct, uint8_t *buf, size_t bufSize);
uint8_t checkReceivedPayload(nrfStruct_t *nrfStruct, uint8_t pipe);

/* Modes */
void modeRX(nrfStruct_t *nrfStruct);
void modeTX(nrfStruct_t *nrfStruct);
void modeStandby(nrfStruct_t *nrfStruct);

/* Interrupts */
void disableTXinterrupt(nrfStruct_t *nrfStruct);    //mask data send interrupt
void disableRXinterrupt(nrfStruct_t *nrfStruct);    //mask data receive interrupt
void disableMaxRTinterrupt(nrfStruct_t *nrfStruct); //mask data retransmition interrupt

void enableTXinterrupt(nrfStruct_t *nrfStruct); //dont mask data send interrupt
void enableRXinterrupt(nrfStruct_t *nrfStruct); //dont mask data receive interrupt
void enableRTinterrupt(nrfStruct_t *nrfStruct); //dont mask data retransmition interrupt

void clearRX_DR(nrfStruct_t *nrfStruct); //clear irt bits in Status Register
void clearTX_DS(nrfStruct_t *nrfStruct);
void clearMAX_RT(nrfStruct_t *nrfStruct);
void clearIrqFlags(nrfStruct_t *nrfStruct);

uint8_t getRX_DR(nrfStruct_t *nrfStruct);
uint8_t getTX_DS(nrfStruct_t *nrfStruct);
uint8_t getMAX_RT(nrfStruct_t *nrfStruct);

/* CRC */
void enableCRC(nrfStruct_t *nrfStruct);
void setCRC(nrfStruct_t *nrfStruct, widthCRC_t w);

/* Auto ACK */
uint8_t checkPipe(uint8_t pipe);
uint8_t enableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableAutoAckPipe(nrfStruct_t *nrfStruct, uint8_t pipe);

/* RX addresses */
uint8_t enableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableRxAddr(nrfStruct_t *nrfStruct, uint8_t pipe);

/* Address Width */
void setAddrWidth(nrfStruct_t *nrfStruct, addressWidth_t width);

/* Setup retransmission */
uint8_t setAutoRetrCount(nrfStruct_t *nrfStruct, uint8_t count);
uint8_t setAutoRetrDelay(nrfStruct_t *nrfStruct, uint8_t delay);

/* RF channel */
uint8_t setChannel(nrfStruct_t *nrfStruct, uint8_t channel);

/* RF setup */
/*
 * @Brief enableContCarrier and enableLockPLL should be use only to RF test
 */
void enableContCarrier(nrfStruct_t *nrfStruct);
void disableContCarrier(nrfStruct_t *nrfStruct);
void enableLockPLL(nrfStruct_t *nrfStruct);
void diableLockPLL(nrfStruct_t *nrfStruct);

void setRFpower(nrfStruct_t *nrfStruct, powerRF_t power);
void setDataRate(nrfStruct_t *nrfStruct, dataRate_t rate);

/* Status */
uint8_t getStatusFullTxFIFO(nrfStruct_t *nrfStruct); //TODO: to tests
uint8_t getPipeStatusRxFIFO(nrfStruct_t *nrfStruct);

/* Transmit observe */
uint8_t lostPacketsCount(nrfStruct_t *nrfStruct); //TODO: to tests
uint8_t retrPacketsCount(nrfStruct_t *nrfStruct); //TODO: to tests
void clearlostPacketsCount(nrfStruct_t *nrfStruct);
/* RPD - for RF test use only */
uint8_t checkRPD(nrfStruct_t *nrfStruct);

/* Receive Address data pipe */
uint8_t setReceivePipeAddress(nrfStruct_t *nrfStruct, uint8_t pipe,
                              uint8_t *addr, size_t addrBufSize);

/* Transmit address data pipe */
uint8_t setTransmitPipeAddress(nrfStruct_t *nrfStruct, uint8_t *addrBuf,
                               size_t addrBufSize);

/* Payload width of each pipes */
uint8_t getRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t setRxPayloadWidth(nrfStruct_t *nrfStruct, uint8_t pipe, uint8_t width);

/* TX and RX FIFO */
uint8_t getRxStatusFIFO(nrfStruct_t *nrfStruct);
uint8_t getTxStatusFIFO(nrfStruct_t *nrfStruct);
uint8_t getTxReuse(nrfStruct_t *nrfStruct); //TODO: to tests

/* DYNPD */
uint8_t enableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe);
uint8_t disableDynamicPayloadLengthPipe(nrfStruct_t *nrfStruct, uint8_t pipe);

/* Feature */
void enableDynamicPayloadLength(nrfStruct_t *nrfStruct);
void disableDynamicPayloadLength(nrfStruct_t *nrfStruct);

void enableAckPayload(nrfStruct_t *nrfStruct);
void disableAckPayload(nrfStruct_t *nrfStruct);

void enableNoAckCommand(nrfStruct_t *nrfStruct); //TODO: to tests
