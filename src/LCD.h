/*
 * LCD.h
 *
 *  Created on: 2017. febr. 24.
 *      Author: MEMS
 */

#ifndef SRC_LCD_H_
#define SRC_LCD_H_


/**************************************************
 *  Includes
**************************************************/
#include <stdio.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_pcnt.h"

#include "display.h"
#include "textdisplay.h"
#include "retargettextdisplay.h"

/* DISPLAY driver inclustions */
#include "displaypal.h"

/**************************************************
 *  Functions
**************************************************/

void InitDisplay();
void PcntInit( void );
void ClearDisplay();
void PrintNewLine(unsigned int n);
void PrintLine(unsigned *N_lineleft);
void PrintFullLineWithChar(unsigned *N_lineleft, char c);


/**************************************************
 *  Defines
**************************************************/
/* Frequency of RTC (COMP0) pulses on PRS channel 2. */
#define RTC_PULSE_FREQUENCY    (LS013B7DH03_POLARITY_INVERSION_FREQUENCY)
#define SLEEP_TIME             (1)

#define N_of_lines				16
#define N_of_chars_per_line 	16

#define LS013B7DH03_CMD_ALL_CLEAR  	(0x04)
#define LS013B7DH03_CMD_UPDATE		(0x01)

#endif /* SRC_LCD_H_ */
