/*
 * ADXL362.h
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */

#ifndef SRC_ADXL362_H_
#define SRC_ADXL362_H_

/**************************************************
 *  Includes
**************************************************/

#include "SPI.h"
#include "SysTick.h"
#include "LCD.h"
#include <stdio.h>

/**************************************************
 *  Functions
**************************************************/
void InitADXL362(void);
void StartADXL362();
unsigned int GetDeviceID();
unsigned int ReadRegister(unsigned int reg);
uint16_t ReadTwoRegisters(unsigned int reg_l);
void WriteRegister(unsigned int reg, unsigned int data);
void readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature);
void DisplayXYZTData(unsigned *N_lineleft);

/**************************************************
 *  Global Defines
**************************************************/

/* Commands */
#define WRITE_REG			0x0A
#define READ_REG			0x0B
#define READ_FIFO			0x0D

/* Accelerometer registers addresses */
#define DEVICE_ID_REG		0x00
#define PART_ID_REG			0x02

#define XDATA				0x08
#define YDATA				0x09
#define ZDATA				0x0A

#define STATUS_REG         	0x0B

#define XDATA_L_REG        	0x0E
#define XDATA_H_REG			0x0F

#define YDATA_L_REG        	0x10
#define YDATA_H_REG			0x11

#define ZDATA_L_REG        	0x12
#define ZDATA_H_REG			0x13

#define TEMP_L_REG         	0x14
#define SOFT_RESET_REG    	0x1F
#define THRESH_ACT_L       	0x20
#define THRESH_ACT_H       	0x21
#define TIME_ACT           	0x22
#define THRESH_INACT_L     	0x23
#define THRESH_INACT_H     	0x24
#define TIME_INACT_L       	0x25
#define TIME_INACT_H       	0x26
#define ACT_INACT_CTL      	0x27
#define INTMAP1            	0x2A
#define INTMAP2            	0x2B
#define POWER_CTL_REG      	0x2D

/* ID register data */
#define DEVICE_ID			0xAD

/* Configuration parameters */
#define TEMP_ADC        1     /* Temperature display units: 1 for ADC, 0 for degrees Celsius */

/* Temperature parameters */
#define ACC_TEMP_BIAS             (float)350         /* Accelerometer temperature bias(in ADC codes) at 25 Deg C */
#define ACC_TEMP_SENSITIVITY      (float)0.065       /* Accelerometer temperature sensitivity  from datasheet (DegC per Code) */

#define SCAN_SENSOR_TIME   500    /* Accelerometer scan interval in ms */

#define ACT_VALUE          50     /* Activity threshold value */

#define INACT_VALUE        50     /* Inactivity threshold value */

#define ACT_TIMER          100    /* Activity timer value in ms */

#define INACT_TIMER        10     /* Inactivity timer value in seconds */

/* Code settings */
#define display_acceleration_mG		//If this is defined the display will use mG units. This is the recommended option.
#define display_acceleration_ms2	//Display in m/s^2

#endif /* SRC_ADXL362_H_ */
