/*
 * SysTick.h
 *
 *  Created on: 23 Feb 2017
 *      Author: Mikl�s
 */

#ifndef SRC_SYSTICK_H_
#define SRC_SYSTICK_H_


/**************************************************
 *  Includes
**************************************************/
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"

/**************************************************
 *  Functions
**************************************************/
void InitSysTick();
void Delay(uint32_t dlyTicks);
void SysTick_Handler(void);

#endif /* SRC_SYSTICK_H_ */
