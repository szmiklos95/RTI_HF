/*
 * SPI.c
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */


#include "SPI.h"

/**************************************************
 *  Sets up the usart for SPI
**************************************************/
void InitUSART(){
    // Set up the necessary peripheral clocks
    CMU_ClockEnable(cmuClock_GPIO, true);
    CMU_ClockEnable(cmuClock_USART1, true);

    // Pin setup is done in GPIO.c

    // Initialize and enable the USART
    USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;
    init.clockMode = usartClockMode0;
    init.msbf = true;

    USART_InitSync(USART1, &init);

    // Connect the USART signals to the GPIO peripheral
    USART1->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN |
                USART_ROUTE_CLKPEN | USART_ROUTE_LOCATION_LOC4;
}
