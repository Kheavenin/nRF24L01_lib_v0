/**
	* @Autor: Khevenin
	* Configuration example for nRF24L01+ library
*/


#define TEST_CONFIG 1
#define TEST_STATIC_LENGTH 1
#define TEST_DYNAMIC_LENGTH 1
#define	TEST_ACK_PAYLOAD 1

#define TAB_SIZE 5
#define BUF_SIZE 32

const uint8_t pipe0 = 0;

uint8_t TransmitAddress[TAB_SIZE] = { 'A', 'B', 'A', 'B', 'A' };
uint8_t ReceiveAddress[TAB_SIZE] = { 'A', 'B', 'A', 'B', 'A' };

uint8_t ReceiveData[BUF_SIZE];
uint8_t TransmitData[BUF_SIZE];

/* HAL init part as example */
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_TIM1_Init();
  MX_USART2_UART_Init(); 		//use for print logs
  /* USER CODE BEGIN 2 */	
  HAL_TIM_Base_Start(&htim1);	//enable timer for microseconds delay - important for SPI communication 
  /* USER CODE END 2 */

/* Before while(1) */ 
#if  TEST_CONFIG
	/* 0. Create pointer and init structure. */
	nrfStruct_t *testStruct;														// create pointer to main struct
	testStruct = nRF_Init(&hspi1, &htim1, CSN_GPIO_Port, CSN_Pin, CE_GPIO_Port,
	CE_Pin);																		// create main nRF24L01 struct

	/* 1.1  Set role as RX */
	modeRX(testStruct);
	/* 1.2 Enable CRC and set coding */
	enableCRC(testStruct);
	setCRC(testStruct, CRC_16_bits);	
	/* 1.3 Enable/disable interrupts */
	enableRXinterrupt(testStruct);	//non mask interrupt 
	enableTXinterrupt(testStruct);
	/* 2. Set ACK for RX pipe  */
	enableAutoAckPipe(testStruct, pipe0);
	/* 3. Set RX pipe */
	enableRxAddr(testStruct, pipe0);
	/* 4. Set RX/TX address width */
	setAddrWidth(testStruct, longWidth);
	/* 5. Set ARD and ARC */
	setAutoRetrCount(testStruct, 4); // 
	setAutoRetrDelay(testStruct, 5); // 5 x 250us 
	/* 6. Set RF channel */
	setChannel(testStruct, 2);
	/* 7. Set RF power and Data Rate */
	setRFpower(testStruct, RF_PWR_6dBm); 		// Output power level -6dBm
	setDataRate(testStruct, RF_DataRate_250);	// Data rate 250kbps
	/* 8 Set RX address */
	setReceivePipeAddress(testStruct, pipe0, ReceiveAddress,
			sizeof(ReceiveAddress));
	/* 9. Set TX address */
	setTransmitPipeAddress(testStruct, TransmitAddress,
			sizeof(TransmitAddress));

#if TEST_STATIC_LENGTH
	setRxPayloadWidth(testStruct, pipe0, BUF_SIZE);
	sendString("nRF24L01+ init done\r\n", &huart2);
#endif

#if TEST_DYNAMIC_LENGTH
	enableDynamicPayloadLength(testStruct);
	enableDynamicPayloadLengthPipe(testStruct, pipe0);
#endif

#if TEST_ACK_PAYLOAD
	enableAckPayload(testStruct);
	writeTxPayloadAck(testStruct, TransmitData, sizeof(TransmitData));
#endif
	
#endif