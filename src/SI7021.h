/*
 * SI7013.h
 *
 *  Created on: 2017. márc. 10.
 *      Author: MEMS
 */

#ifndef SRC_SI7021_H_
#define SRC_SI7021_H_

/**************************************************
 *  Includes
**************************************************/

#include "i2cspm.h"
#include <stdio.h>
#include "em_gpio.h"
#include "em_cmu.h"
#include "si7013.h"

/**************************************************
 *  Functions
**************************************************/
void InitSI7021();
void MeasureHumAndTemp(I2C_TypeDef *i2c, uint32_t *rhData, int32_t *tData);
void DisplayHumTempData(unsigned *N_lineleft);

#endif /* SRC_SI7021_H_ */
