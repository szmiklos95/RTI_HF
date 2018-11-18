/*
 * SPI.h
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */

#ifndef SRC_SPI_H_
#define SRC_SPI_H_

/**************************************************
 *  Includes
**************************************************/
#include "em_usart.h"
#include "em_cmu.h"
#include "em_gpio.h"

/**************************************************
 *  Functions
**************************************************/

void InitUSART();

/**************************************************
 *  Global Defines
**************************************************/

#define true 1
#define false 0


/**************************************************
 *  ADXL362 Accelerometer
 *  Setup details:
 *  - The recommended SPI clock speeds are 1 MHz to 8 MHz, with 12 pF maximum loading.
 *  - The SPI timing scheme follows CPHA = CPOL = 0
**************************************************/


#endif /* SRC_SPI_H_ */
