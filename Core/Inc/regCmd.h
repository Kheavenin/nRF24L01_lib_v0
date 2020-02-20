

/**
 * @Autor: Khevenin
 * Register list of nRF24L01+ and deafult value of registers.
 * All addresses are 8 bits.
*/

/**
 * Map rregisters
 **/

/** Control registers */
#define CONFIG 0x00		//PWR, role, interrupts
#define EN_AA 0x01		//enable auto ack
#define EN_RXADDR 0x02  //enable RX addresses
#define SETUP_AW 0x03   //setup of address widths
#define SETUP_RETR 0x04 //setup of automatic retrasmission
#define RF_CH 0x05		//RF channel
#define RF_SETUP 0x06   //RF setup register
#define STATUS 0x07		//Status register
#define OBSERVE_TX 0x08 //Transmit observe register
#define RPD 0x09		//Received Power Detector

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
#define DYNPD 0x1C		 //Enable dynamic payload lenght
#define FEATURE 0x1D	 // Feature register

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
#define DF_STATUS 0x0E	 // B - 3:0 - read only
#define DF_OBSERVE_TX 0x00 //read only
#define DF_RPD 0x00		   //read only

#define DF_RX_ADDR_P0 0xE7E7E7E7E7
#define DF_RX_ADDR_P1 0xC2C2C2C2C2
#define DF_RX_ADDR_P2 0xC3
#define DF_RX_ADDR_P3 0xC4
#define DF_RX_ADDR_P4 0xC5
#define DF_RX_ADDR_P5 0xC6

#define DF_TX_ADDR 0xE7E7E7E7E7

#define DF_RX_PW_P0 0x00
#define DF_RX_PW_P1 0x00
#define DF_RX_PW_P2 0x00
#define DF_RX_PW_P3 0x00
#define DF_RX_PW_P4 0x00
#define DF_RX_PW_P5 0x00

#define DF_FIFO_STATUS 0x11 // B - 6:4, 1:0 - read only
#define DF_DYNPD 0x00
#define DF_FEATURE 0x00

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

/* Config register */
#define MASK_RX_DR bit6
#define MASK_TX_DS bit5
#define MASK_MAX_RT bit4
#define EN_CRC bit3
#define CRCO bit2
#define PWR_UP bit1
#define NPRIM_RX bit0

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

typedef enum
{
	RF_PWR_0dBm = 0x03,
	RF_PWR_6dBm = 0x02,
	RF_PWR_12dBm = 0x01,
	RF_PWR_18dBm = 0x00
} powerRF_t;

typedef enum
{
	RF_DataRate_250 = 0x04,
	RF_DataRate_1M = 0x00,
	RF_DataRate_2M = 0x01
} dataRate_t;

typedef enum
{
	CRC_8_bits = 0,
	CRC_16_bits = 1
} widthCRC_t;

typedef enum
{
	shortWidth = 0x01,
	mediumWidth = 0x02,
	longWidth = 0x03
} addressWidth_t;

/*
 * Funtions return codes
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
