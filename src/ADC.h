/*
 * ADC.h
 *
 *  Created on: 2017. márc. 31.
 *      Author: MEMS
 */

#ifndef SRC_ADC_H_
#define SRC_ADC_H_

/**************************************************
 *  Includes
**************************************************/

#include "em_adc.h"
#include "em_cmu.h"
#include "LCD.h"

/**************************************************
 *  Functions
**************************************************/
void InitADC();
uint32_t GetADCvalue(void);
void DisplayADCvalue(unsigned *N_lineleft);
void DrawADCvalue(unsigned *N_lineleft, char c);

#endif /* SRC_ADC_H_ */
