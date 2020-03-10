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

void settingStruct_Init(nrfStruct_t *nrfStruct);
void addressStruct_Init(nrfStruct_t *nrfStruct);
void fifoStruct_Init(nrfStruct_t *nrfStruct);
void hardware_Init(nrfStruct_t *nrfStruct, SPI_HandleTypeDef *HAL_SPIx,
		TIM_HandleTypeDef *HAL_TIMx, GPIO_TypeDef *HAL_GPIO_CSN,
		uint16_t HAL_GPIO_Pin_CSN, GPIO_TypeDef *HAL_GPIO_CE,
		uint16_t HAL_GPIO_Pin_CE);

#endif /* INC_HIGHLEVELMODULE_H_ */
