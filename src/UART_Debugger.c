/*
 * UART_Debugger.c
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */

#include "UART_Debugger.h"

#define DEBUG_BREAK           __asm__("BKPT #0");

/*************************************************************
 * Initializes everything.
 *************************************************************/
void InitDebugger(){
#ifdef RUN_DEBUG
	  InitUART();
	  InitDebug();
#endif
}

/*************************************************************
 * Contains everything that is needed to initialize the uart.
 *************************************************************/
void InitUART(){
#ifdef RUN_DEBUG
	/* USART is a HFPERCLK peripheral. Enable HFPERCLK domain and USART1.
	 * We also need to enable the clock for GPIO to configure pins. */
	CMU_ClockEnable(cmuClock_HFPER, true);
	CMU_ClockEnable(cmuClock_USART1, true);
	CMU_ClockEnable(cmuClock_GPIO, true); //Enable GPIO peripheral


	/* Initialize with default settings and then update fields according to application requirements. */
	USART_InitAsync_TypeDef initAsync = USART_INITASYNC_DEFAULT;
	initAsync.baudrate = 38400;
	USART_InitAsync(USART1, &initAsync);



	/* Enable I/O and set location */
	USART1->ROUTE = USART_ROUTE_RXPEN | USART_ROUTE_TXPEN | USER_LOCATION;
	/* Also enable CS and CLK pins if the USART is configured for synchronous mode.
	 * Set GPIO mode. */

	 /* To avoid false start, configure TX pin as initial high */
	 GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART1_TX_PORT(USER_LOCATION), AF_USART1_TX_PIN(USER_LOCATION), gpioModePushPull, 1);
	 GPIO_PinModeSet((GPIO_Port_TypeDef)AF_USART1_RX_PORT(USER_LOCATION), AF_USART1_RX_PIN(USER_LOCATION), gpioModeInput, 0);
	 /* Don't enable CTS/RTS hardware flow control pins in this example. */

	  USART_IntClear(USART1, USART_IF_RXDATAV);
	  USART_IntEnable(USART1, USART_IF_RXDATAV);

	  NVIC_ClearPendingIRQ(USART1_RX_IRQn);
	  NVIC_EnableIRQ(USART1_RX_IRQn);


	  USART_IntClear(USART1, USART_IF_TXC);
	  USART_IntEnable(USART1, USART_IF_TXC);

	  NVIC_ClearPendingIRQ(USART1_TX_IRQn);
	  NVIC_EnableIRQ(USART1_TX_IRQn);
#endif
}


/*************************************************************
 * The interrupt handler for received packages.
 *************************************************************/
void USART1_RX_IRQHandler(void){
#ifdef RUN_DEBUG
	if (USART1->IF & USART_IF_RXDATAV)
	      {
	            char test_char = USART_Rx(USART1);
	            USART_Tx(USART1, test_char);
	      }
#endif
}

/*************************************************************
 * The interrupt handler for sending packages.
 *************************************************************/
void USART1_TX_IRQHandler(void){
#ifdef RUN_DEBUG //If we need to debug
if (USART1->IF & USART_IF_TXC)
      {
            // This flag is not automatically cleared like RXDATAV
            USART_IntClear(USART1, USART_IF_TXC);

            if (buffer.tail != buffer.head)
            {
                  USART_Tx(USART1, buffer.data[buffer.tail++]);
                  buffer.tail = fix_overflow(buffer.tail);
            }
            else
            {
                  buffer.now_printing = false;
            }
      }
#endif
}


/**************************************************
 *  Gives starting value for the buffer variable.
**************************************************/
void InitDebug(){
#ifdef RUN_DEBUG
	for(int i = 0; i<PRINT_BUFFER_SIZE;++i){
		buffer.data[i]=0;
	}
	buffer.head=0;
	buffer.tail=0;
	buffer.now_printing=false;
#endif
}

/**************************************************
 *  Handles overflow
**************************************************/
uint16_t fix_overflow(uint16_t index){
#ifdef RUN_DEBUG
	if (index >= PRINT_BUFFER_SIZE)
	{
		return 0;
	}
	return index;
#endif
return 0;
}

/**************************************************
 *  Prepares the string to be sent out.
**************************************************/
void send_string(char * string){
#ifdef RUN_DEBUG
      while (*string != 0)
      {
            if (*string == '\n')
            {
                  //USART_Tx(USART1, '\r');
                  buffer.data[buffer.head++] = '\r';
                  buffer.head = fix_overflow(buffer.head);
            }
            //USART_Tx(USART1, *string++);

            buffer.data[buffer.head++] = *string++;
            buffer.head = fix_overflow(buffer.head);
      }

      // We need to kick off the first transfer sometimes to get things going
      CORE_AtomicDisableIrq();
      if (!buffer.now_printing)
      {
            buffer.now_printing = true;
            USART_Tx(USART1, buffer.data[buffer.tail++]);
            buffer.tail = fix_overflow(buffer.tail);
      }
      CORE_AtomicEnableIrq();
#endif
}
