/*
 * GPIO.h
 *
 *  Created on: 20 Feb 2017
 *      Author: Miklós
 */

/***************** TUTORIAL *****************/
/*			 How to add new GPIO
 *
 * In this example I am going to go through the steps needed to setup the ADC0 analog input pin on PD4.
 * The ADC setup is located in ADC.h but we will not need those here. We only need to know that we want to use PD4.
 *
 * The steps:
 * 1.) In GPIO.h (this file) add the following lines:
 * //--------ADC--------//
 *	#define 	ADC0_PORT		gpioPortD
 *	#define 	ADC0_PIN		4
 *
 * 2.) In GPIO.h (at the structs field) add the following:
 * GPIO_struct ADC_0;
 *
 * 3.) In GPIO.c make a new static function to give starting value to the struct made in 2.)
 * static void InitADC(){
 *	SetGPIO_struct(ADC_0, ADC0_PORT, ADC0_PIN, gpioModeInput, 0, false);
 * }
 *
 * 4.) Call this static function in InitGPIO
 *
 * 5.) Build and upload, if no error messages showed up on the LCD it is all set.
 */







#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_


/*************************************************
 * Includes
 **************************************************/
#include "em_cmu.h"
#include "em_gpio.h"
#include "Error.h"





/*************************************************
 * Defines
 **************************************************/
#define 	true 	1
#define 	false 	0
#define 	TRUE 	1
#define 	FALSE 	0

#define MAX_GPIO_COUNT 			37
#define NUMBER_OF_SPI_PINS		4


//---------------- GPIO ----------------//

//--------Buttons--------//
#define		Button1_PORT			gpioPortF
#define 	Button1_PIN				3

//--------SPI--------//
#define 	SPI_CS_PORT		gpioPortE
#define 	SPI_CS_PIN		13
#define 	SPI_MOSI_PORT	gpioPortF
#define 	SPI_MOSI_PIN	2
#define 	SPI_MISO_PORT	gpioPortA
#define 	SPI_MISO_PIN	0
#define 	SPI_CLK_PORT	gpioPortB
#define 	SPI_CLK_PIN		11


//--------SI7021--------//
#define		SI7021_SENSOR_ISOLATION_SWITCH_PORT		gpioPortC
#define		SI7021_SENSOR_ISOLATION_SWITCH_PIN		8


//--------ADC--------//
#define 	ADC0_PORT		gpioPortD
#define 	ADC0_PIN		4


/*************************************************
 * Structs
 **************************************************/

typedef struct GPIO_struct{
	GPIO_Port_TypeDef port;
	unsigned int pin;
	GPIO_Mode_TypeDef mode;
	unsigned int out;
	unsigned int isSet;
}GPIO_struct;

GPIO_struct EveryGPIO[MAX_GPIO_COUNT]; //struct that stores every setup gpio

GPIO_struct Button1; //struct that stores 1 push button setup
GPIO_struct SPI[NUMBER_OF_SPI_PINS];
GPIO_struct SI7021;
GPIO_struct ADC_0;

/*************************************************
 * Functions
 **************************************************/
void InitGPIO();
void SetGPIO(GPIO_Port_TypeDef port, unsigned int pin, unsigned int data);
unsigned GetGPIO(GPIO_Port_TypeDef port, unsigned int pin);
void GPIOSetup(GPIO_struct gpio);
void GPIO_Unified_IRQ(void);
void GPIO_ODD_IRQHandler(void);
unsigned StartMeasurement();

#endif /* SRC_GPIO_H_ */
