/*
 * GPIO.c
 *
 *  Created on: 20 Feb 2017
 *      Author: Miklós
 */

#include "GPIO.h"

/******************************************************************************************
 * 				STATIC VARIABLES
 *******************************************************************************************/
unsigned int EveryGPIO_member_count = 0; //The number of GPIOs initialized so far
unsigned int startMeas = 0;

/******************************************************************************************
 * 				STATIC FUNCTIONS
 *******************************************************************************************/

static void SetGPIO_struct(GPIO_struct this, GPIO_Port_TypeDef port, unsigned int pin, GPIO_Mode_TypeDef mode, unsigned int out, unsigned int isSet ){
	this.port = port;
	this.pin = pin;
	this.mode = mode;
	this.out = out;
	this.isSet = isSet;

	GPIOSetup(this);
}


//This function initilizes the buttons
static void InitButtons(){
	SetGPIO_struct(Button1, Button1_PORT, Button1_PIN, gpioModeInput, 0, false);
}

//This function gives a starting value to the struct variable "EveryGPIO"
static void Setup_EveryGPIO_struct(){
	for(int i=0; i<MAX_GPIO_COUNT; ++i){
		EveryGPIO[i].port = 0;
		EveryGPIO[i].pin = 0;
		EveryGPIO[i].mode = 0;
		EveryGPIO[i].out = 0;
		EveryGPIO[i].isSet = false; //Set every value to signal that no GPIO has been set so far
	}
}

//This functions saves a GPIO_struct to the next place in EveryGPIO
static void SaveToEveryGPIO(GPIO_struct this){
	EveryGPIO[EveryGPIO_member_count].port = this.port;
	EveryGPIO[EveryGPIO_member_count].pin = this.pin;
	EveryGPIO[EveryGPIO_member_count].mode = this.mode;
	EveryGPIO[EveryGPIO_member_count].out = this.out;
	EveryGPIO[EveryGPIO_member_count].isSet = this.isSet;

	EveryGPIO_member_count++;
}

/* This functions returns false if a GPIO cannot be used.
 * Possible causes: - The GPIO is already set by user.
 * 					- The GPIO is on the list of unavaialble GPIOs
 */
static unsigned isGPIOAvailable(GPIO_struct gpio){
	for(int i=0; i<EveryGPIO_member_count; ++i){
		if(EveryGPIO[i].isSet){ //If there is an already set gpio
			if(gpio.port == EveryGPIO[i].port){ //If it is on the same port
				if(gpio.pin == EveryGPIO[i].pin){ //And also on the same pin
					return false; //this gpio is already taken and cannot be used
				}
			}
		}
	}
	return true;
}


//This function initializes the SPI pins
static void InitSPI(){
    // Enable the GPIO pins for the USART, starting with CS
    // This is to avoid clocking the ic when we set CLK high
	SetGPIO_struct(SPI[0], SPI_CS_PORT, SPI_CS_PIN, gpioModePushPull, 1, false);
	SetGPIO_struct(SPI[1], SPI_MOSI_PORT, SPI_MOSI_PIN, gpioModePushPull, 0, false);
	SetGPIO_struct(SPI[2], SPI_MISO_PORT, SPI_MISO_PIN, gpioModeInput, 0, false);
	SetGPIO_struct(SPI[1], SPI_CLK_PORT, SPI_CLK_PIN, gpioModePushPull, 0, false);
}

static void InitSI7021(){
	SetGPIO_struct(SI7021, SI7021_SENSOR_ISOLATION_SWITCH_PORT, SI7021_SENSOR_ISOLATION_SWITCH_PIN, gpioModePushPull, 1, false);
}

static void InitADC(){
	SetGPIO_struct(ADC_0, ADC0_PORT, ADC0_PIN, gpioModeInput, 0, false);
}

//Sets up interrupt for the desired pins
static void SetupInterrupt(){
	  GPIO_IntConfig(Button1_PORT, Button1_PIN, true, true, true); //IT for both rising and falling edge
	  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
	  NVIC_EnableIRQ(GPIO_ODD_IRQn);
}

/******************************************************************************************
 * 					FUNCTIONS
 *******************************************************************************************/





/*************************************************
 * Initializes GPIO and our pins
 **************************************************/
void InitGPIO(){
	CMU_ClockEnable(cmuClock_GPIO, true); //Enable GPIO peripheral

	Setup_EveryGPIO_struct(); //Initializes the struct
	InitButtons();
	InitSPI();
	InitSI7021();
	InitADC();
	SetupInterrupt();
}

/*************************************************
 * If data is 1 set the output the given pin
 *  (on the given port) to be 1. Else it is 0.
 **************************************************/
void SetGPIO(GPIO_Port_TypeDef port, unsigned int pin, unsigned int data){
	if(data==1) GPIO_PinOutSet(port, pin);
	else 		GPIO_PinOutClear(port, pin);
}

/*************************************************
 * Reads the value of a pin.
 **************************************************/
unsigned GetGPIO(GPIO_Port_TypeDef port, unsigned int pin){
	return GPIO_PinInGet(port,pin);
}


/*************************************************
 * Uses my own structure to setup a gpio
 **************************************************/
void GPIOSetup(GPIO_struct gpio){
	if(isGPIOAvailable(gpio)){
		GPIO_PinModeSet(gpio.port, gpio.pin, gpio.mode, gpio.out);
		gpio.isSet = true;
		SaveToEveryGPIO(gpio);
	}
	else{ //Error handling
		GPIOError(gpio.port, gpio.pin);
	}
}


/**************************************************************************//**
 * @brief Unified GPIO Interrupt handler (pushbuttons).
 *****************************************************************************/
void GPIO_Unified_IRQ(void)
{
  // Get and clear all pending GPIO interrupts.
  uint32_t interruptMask = GPIO_IntGet();
  GPIO_IntClear(interruptMask);

  // Act on interrupts.
  if (interruptMask & (1 << Button1_PIN))
  {
    startMeas = GetGPIO(Button1_PORT, Button1_PIN);
  }
}

/**************************************************************************//**
 * @brief GPIO Interrupt handler for odd pins.
 *****************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  GPIO_Unified_IRQ();
}


/*************************************************
 * Returns if the button has been pushed
 **************************************************/
unsigned StartMeasurement(){
	return startMeas;
}
