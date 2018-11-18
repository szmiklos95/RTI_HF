/*
 * ADXL362.c
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */


#include "ADXL362.h"
#define DEBUG_BREAK           __asm__("BKPT #0");

/******************************************************************************************************************
 *  Static variables
******************************************************************************************************************/

static int16_t XData = 0;
static int16_t YData = 0;
static int16_t ZData = 0;
static int16_t Temperature = 0;

/******************************************************************************************************************
 *  Static functions
******************************************************************************************************************/

static void convert_g_to_ms2(int16_t *temp_XData, int16_t *temp_YData, int16_t *temp_ZData){
	*temp_XData = XData * 9.81 / 1000;
	*temp_YData = YData * 9.81 / 1000;
	*temp_ZData = ZData * 9.81 / 1000;
}


/******************************************************************************************************************
 *  Functions
******************************************************************************************************************/

/**************************************************
 *  Sets up the accelerometer.
**************************************************/
void InitADXL362(){
	WriteRegister(SOFT_RESET_REG, 0x52); /* Soft reset accelerometer */
	Delay(1);

	/* Set activity threshold */
	//WriteRegister(THRESH_ACT_L, ACT_VALUE & 0xFF);
	//WriteRegister(THRESH_ACT_H, ACT_VALUE >> 8);

	//WriteRegister(TIME_ACT, (ACT_TIMER / 10));     /* Set activity time at 100Hz data rate */

	/* Set inactivity threshold */
	//WriteRegister(THRESH_INACT_L, INACT_VALUE & 0xFF);
	//WriteRegister(THRESH_INACT_H, INACT_VALUE >> 8);


	/* Set inactivity time at 100Hz data rate */
	//WriteRegister(TIME_INACT_L, ((INACT_TIMER * 100) & 0xFF));
	//WriteRegister(TIME_INACT_H, ((INACT_TIMER * 100) >> 8));

	//WriteRegister(ACT_INACT_CTL, 0x3F);         /* Set Loop mode, referenced mode for activity and inactivity, enable activity and inactivity functionality */


#if(ADXL_INT_SEL == INTACC_PIN_1)
	//WriteRegister(INTMAP1, 0x40);                  /* Map the awake status to INT1 pin */
#elif(ADXL_INT_SEL == INTACC_PIN_2)
	//WriteRegister(INTMAP2, 0x40);                  /* Map the awake status to INT2 pin */
#endif

StartADXL362();
}


/**************************************************
 * Turns on measurement mode
**************************************************/
void StartADXL362(){
	unsigned temp = ReadRegister(POWER_CTL_REG); /* Read POWER_CTL register, before modifying it */
	temp |= 0x02;  /* Set measurement bit in POWER_CTL register */
	WriteRegister(POWER_CTL_REG, temp); /* Write the new value to POWER_CTL register */
	temp = ReadRegister(POWER_CTL_REG); /* Read POWER_CTL register, before modifying it */
}





/**************************************************
 * Returns the Device ID if it is the specified one.
**************************************************/
unsigned int GetDeviceID(){
    uint8_t ID;

    GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); //CS down

	USART_SpiTransfer(USART1, READ_REG);	//send read command
	USART_SpiTransfer(USART1, DEVICE_ID_REG); //send register address

	ID = USART_SpiTransfer(USART1, 0);

	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); //CS up

	if(ID == DEVICE_ID){
		return ID;
	}

	return 0;
}

/**************************************************
 * Returns the data of the given register.
**************************************************/
unsigned int ReadRegister(unsigned int reg){
		uint8_t data = 0;

		GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); //CS down

		USART_SpiTransfer(USART1, READ_REG);	//send read command
		USART_SpiTransfer(USART1, reg); //send register address

		data = USART_SpiTransfer(USART1, 0);
		//data[1] = USART_SpiTransfer(USART1, 0);
		//data[2] = USART_SpiTransfer(USART1, 0);

		GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); //CS up

		return data;
}


/**************************************************
 * Returns the data of the given register + 1
**************************************************/
uint16_t ReadTwoRegisters(unsigned int reg_l){
	uint8_t data[2] = {0,0};
	uint16_t data16 = 0;
	uint8_t mask = 0b00001111; //The upper 4 bits are only extended bits

	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); //CS down

	USART_SpiTransfer(USART1, READ_REG);	//send read command
	USART_SpiTransfer(USART1, reg_l); //send register address

	data[0] = USART_SpiTransfer(USART1, 0);
	data[1] = USART_SpiTransfer(USART1, 0);
	data[1] &= mask; //We meed to maks the upper 4 bits

	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); //CS up

	data16 = (uint16_t)((data[1] << 8) | data[0]);
	return data16;
}


/**************************************************
 * Writes 1 byte to a register
**************************************************/
void WriteRegister(unsigned int reg, unsigned int data){
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); //CS down
	USART_SpiTransfer(USART1, WRITE_REG);
	USART_SpiTransfer(USART1, reg);
	USART_SpiTransfer(USART1, data);
	//Wait for 3 data to arrive
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); //CS up
}


/**************************************************
 * Burst read registers.
 * A burst read of all three axis is required to
 * guarantee all measurements correspond to same sample time
**************************************************/
void readXYZTData(int16_t *XData, int16_t *YData, int16_t *ZData, int16_t *Temperature){
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 0); //CS down
	USART_SpiTransfer(USART1, READ_REG);  // read instruction
	USART_SpiTransfer(USART1, XDATA_L_REG);  // Start at XData Reg
	*XData = USART_SpiTransfer(USART1, 0);
	*XData = *XData + (USART_SpiTransfer(USART1, 0) << 8);
	*YData = USART_SpiTransfer(USART1, 0);
	*YData = *YData + (USART_SpiTransfer(USART1, 0) << 8);
	*ZData = USART_SpiTransfer(USART1, 0);
	*ZData = *ZData + (USART_SpiTransfer(USART1, 0) << 8);
	*Temperature = USART_SpiTransfer(USART1, 0);
	*Temperature = *Temperature + (USART_SpiTransfer(USART1, 0) << 8);
	*Temperature = (*Temperature + ACC_TEMP_BIAS)/(1 / ACC_TEMP_SENSITIVITY); //ADC value conversion to celsius
	GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1); //CS up
}

/**************************************************
 * Displays the X,Y,Z and Temperature data
 * plus n_emptyline number of empty lines.
**************************************************/
void DisplayXYZTData(unsigned *N_lineleft){
	PrintNewLine(1);
	readXYZTData(&XData, &YData, &ZData, &Temperature);
#ifdef display_acceleration_ms2
	int16_t temp_XData = 0;
	int16_t temp_YData = 0;
	int16_t temp_ZData = 0;
	convert_g_to_ms2(&temp_XData, &temp_YData, &temp_ZData);
	printf("Xdata:%d[m/s2]\n", temp_XData);
	printf("Ydata:%d[m/s2]\n", temp_YData);
	printf("Zdata:%d[m/s2]\n", temp_ZData);
	*N_lineleft -= 3;
	//PrintFullLineWithChar(N_lineleft, '-');
#endif
#ifdef display_acceleration_mG
	printf("Xdata:%d[mG]\n", XData);
	printf("Ydata:%d[mG]\n", YData);
	printf("Zdata:%d[mG]\n", ZData);
	*N_lineleft -= 3;
	//PrintFullLineWithChar(N_lineleft, '-');
#endif
	printf("Tdata:%d['C]\n", Temperature);
	*N_lineleft -=2;
	//PrintNewLine(*N_lineleft);
}
