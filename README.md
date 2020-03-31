nRF24L01_lib_v0 

Nordic Semiconductor transceiver nRF24L01+ library.
Library was create for STM32-Cortex Mx microcontrollers and it is base HAL libraries. 
Tested on STM32F103 and STM32F411.

Before use:
- Read nRF24L01+ datasheet. It's important to know how this chip work.
- If you have experience with Arduino and nRF24l01+ it's good practice. It will be helpful. 
- Check twice configuration code and settings - if something don't work probably config is wrong.
- Remember about ARD while you use 32 bytes payload with 32 bytes ACK payload. If auto retransmission delay is to short, it can cuts your ACK payload. ARD set greater than 5 is save. 

Datasheet:
https://www.sparkfun.com/datasheets/Components/SMD/nRF24L01Pluss_Preliminary_Product_Specification_v1_0.pdf
