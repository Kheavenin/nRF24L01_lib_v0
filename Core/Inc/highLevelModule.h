/*
 * highLevelModule.h
 *
 *  Created on: 7 mar 2020
 *      Author: Khevenin
 */

#ifndef INC_HIGHLEVELMODULE_H_
#define INC_HIGHLEVELMODULE_H_

#include "settingModule.h"

nrfStruct_t* nRF_Init(SPI_HandleTypeDef *HAL_SPIx, TIM_HandleTypeDef *HAL_TIMx,
		GPIO_TypeDef *HAL_GPIO_CSN, uint16_t HAL_GPIO_Pin_CSN,
		GPIO_TypeDef *HAL_GPIO_CE, uint16_t HAL_GPIO_Pin_CE);

void fifoStruct_Init();
void addressStruct_Init();
void settingStruct_Init();


#endif /* INC_HIGHLEVELMODULE_H_ */
