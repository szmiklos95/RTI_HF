/*
 * Error.h
 *
 *  Created on: 2017. márc. 31.
 *      Author: MEMS
 */

#ifndef SRC_ERROR_H_
#define SRC_ERROR_H_

/*************************************************
 * Includes
 **************************************************/
#include "LCD.h"
#include "em_cmu.h"
#include "em_gpio.h"

/*************************************************
 * Error numbers
 **************************************************/

#define 	DEFAULT_ERROR 		0
#define 	GPIO_ERROR_NUMBER 	1

/*************************************************
 * Functions
 **************************************************/
void PrintAndAbort(unsigned int errnum);
void GPIOError(GPIO_Port_TypeDef port, unsigned int pin);

#endif /* SRC_ERROR_H_ */
