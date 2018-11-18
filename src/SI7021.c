/*
 * SI7013.c
 *
 *  Created on: 2017. márc. 10.
 *      Author: MEMS
 */


#include "SI7021.h"

/******************************************************************************************************************
 *  Static variables
******************************************************************************************************************/

static 	uint32_t 	humData;
static 	int32_t  	tData;
static I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

/*********************************************************************
 * Initializes SI7021 temperature and humidity sensor
*********************************************************************/
void InitSI7021(){
	/* Enable GPIO clock */
	  CMU_ClockEnable(cmuClock_GPIO, true);

	  /* Enable si7021 sensor isolation switch */
	  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 1);

	  I2CSPM_Init(&i2cInit);
}


/*********************************************************************
 * Uses the on-board SI7021 to measure humidity and temperature
*********************************************************************/
void MeasureHumAndTemp(I2C_TypeDef *i2c, uint32_t *humData, int32_t *tData){
	Si7013_MeasureRHAndTemp(i2c, SI7021_ADDR, humData, tData);
}


/*********************************************************************
 * Uses the on-board SI7021 to measure humidity and temperature
*********************************************************************/
void DisplayHumTempData(unsigned *N_lineleft){
	MeasureHumAndTemp(i2cInit.port, &humData, &tData);
	printf("Hum:%ld[%%]\n", humData/1000);
	printf("Temp:%lu['C]\n", tData/1000);
	*N_lineleft-=2;
}








