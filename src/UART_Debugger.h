/*
 * UART_Debugger.h
 *
 *  Created on: 23 Feb 2017
 *      Author: Miklós
 */

#ifndef SRC_UART_DEBUGGER_H_
#define SRC_UART_DEBUGGER_H_

/**************************************************
 *  Includes
**************************************************/
#include "em_cmu.h"
#include "em_usart.h"
#include "em_gpio.h"
#include <stdio.h>
#include <stdarg.h>
#include "em_core.h"

/**************************************************
 *  Functions
**************************************************/
void InitDebugger();

void InitUART();
void USART1_RX_IRQHandler(void);
void USART1_TX_IRQHandler(void);

void InitDebug();
uint16_t fix_overflow(uint16_t index);
void send_string(char * string);

/**************************************************
 *  Global Defines
**************************************************/
#define USER_LOCATION 0 //This is where the USART is physically located. Somehow Loc3 doesn't work with this setup.

//#define RUN_DEBUG //if this is defined all the debug functions are working

#define PRINT_BUFFER_SIZE     512

#define 	true 	1
#define 	false 	0
#define 	TRUE 	1
#define 	FALSE 	0

/**************************************************
 *  Structs
**************************************************/
typedef struct print_buffer
{
      uint16_t head;
      uint16_t tail;
      uint8_t data[PRINT_BUFFER_SIZE];
      uint8_t now_printing;
} print_buffer_struct;

/**************************************************
 *  Variables
**************************************************/


volatile uint32_t msTicks; // Counts 1ms timeTicks
print_buffer_struct buffer;

#endif /* SRC_UART_DEBUGGER_H_ */
