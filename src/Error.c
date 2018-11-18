/*
 * Error.c
 *
 *  Created on: 2017. márc. 31.
 *      Author: MEMS
 */

#include "Error.h"

void PrintAndAbort(unsigned int errnum){
	switch(errnum){
	case GPIO_ERROR_NUMBER: printf("***\nGPIO error. This pin is already taken or is on the list of forbidden gpio.\n***"); break;
	default: printf("\n***\n");
	}
	while(1){

	}
}


void GPIOError(GPIO_Port_TypeDef port, unsigned int pin){
	printf("\n***\nGPIO error \nPORT: ");
	switch(port){
		case gpioPortA: printf("A"); break;
		case gpioPortB: printf("B"); break;
		case gpioPortC: printf("C"); break;
		case gpioPortD: printf("D"); break;
		case gpioPortE: printf("E"); break;
		case gpioPortF: printf("F"); break;
	}
	printf("\nPIN: %d", pin);

	PrintAndAbort(DEFAULT_ERROR);
}
