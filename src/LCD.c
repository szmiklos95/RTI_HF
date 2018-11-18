/*
 * LCD.c
 *
 *  Created on: 2017. febr. 24.
 *      Author: MEMS
 */

#include "LCD.h"


/**************************************************
 *  Variables
**************************************************/

static volatile bool      displayEnabled = false; /* Status of LCD display. */
static volatile uint32_t  seconds = 0;     /* Seconds elapsed since reset.  */

static DISPLAY_Device_t displayDevice;    /* Display device handle.         */

static uint8_t        lcdPolarity = 0;


/**************************************************
 *  Static functions
**************************************************/




/**************************************************************************//**
 * @brief  Clear the display.
 *
 * @detail  This function clears the display.
 *
 * @return  EMSTATUS code of the operation.
 *****************************************************************************/
static EMSTATUS DisplayClear ( void )
{
  uint16_t cmd;

  /* Set SCS */
  PAL_GpioPinOutSet( LCD_PORT_SCS, LCD_PIN_SCS );

  /* SCS setup time: min 6us */
  PAL_TimerMicroSecondsDelay(6);

  /* Send command */
  cmd = LS013B7DH03_CMD_ALL_CLEAR | lcdPolarity;
  PAL_SpiTransmit ((uint8_t*) &cmd, 2 );

  /* SCS hold time: min 2us */
  PAL_TimerMicroSecondsDelay(2);

  /* Clear SCS */
  PAL_GpioPinOutClear( LCD_PORT_SCS, LCD_PIN_SCS );

  return DISPLAY_EMSTATUS_OK;
}


























/**************************************************
 *  Functions
**************************************************/


/**************************************************
 *  Initializes the LCD
**************************************************/
void InitDisplay(){
	  /* Initialize the display module. */
	  displayEnabled = true;
	  DISPLAY_Init();

	  /* Retrieve the properties of the display. */
	  if ( DISPLAY_DeviceGet( 0, &displayDevice ) != DISPLAY_EMSTATUS_OK )
	  {
	    /* Unable to get display handle. */
	    while( 1 );
	  }

	  /* Retarget stdio to the display. */
	  if ( TEXTDISPLAY_EMSTATUS_OK != RETARGET_TextDisplayInit() )
	  {
	    /* Text display initialization failed. */
	    while( 1 );
	  }

	  /* Set PCNT to generate an interrupt every second. */
	  PcntInit();

	  printf("The display is working.\n");
}


/**************************************************************************//**
 * @brief   Set up PCNT to generate an interrupt every second.
 *
 *****************************************************************************/
void PcntInit(void)
{
  PCNT_Init_TypeDef pcntInit = PCNT_INIT_DEFAULT;

  /* Enable PCNT clock */
  CMU_ClockEnable(cmuClock_PCNT0, true);
  /* Set up the PCNT to count RTC_PULSE_FREQUENCY pulses -> one second */
  pcntInit.mode = pcntModeOvsSingle;
  pcntInit.top = RTC_PULSE_FREQUENCY;
  pcntInit.s1CntDir = false;
  /* The PRS channel used depends on the configuration and which pin the
  LCD inversion toggle is connected to. So use the generic define here. */
  pcntInit.s0PRS = (PCNT_PRSSel_TypeDef)LCD_AUTO_TOGGLE_PRS_CH;

  PCNT_Init(PCNT0, &pcntInit);

  /* Select PRS as the input for the PCNT */
  PCNT_PRSInputEnable(PCNT0, pcntPRSInputS0, true);

  /* Enable PCNT interrupt every second */
  NVIC_EnableIRQ(PCNT0_IRQn);
  PCNT_IntEnable(PCNT0, PCNT_IF_OF);
}


/**************************************************************************//**
 * @brief   This interrupt is triggered at every second by the PCNT
 *
 *****************************************************************************/
void PCNT0_IRQHandler(void)
{
  PCNT_IntClear(PCNT0, PCNT_IF_OF);

  seconds++;

  return;
}



/**************************************************
 *  Clears the display, but continues to draw on
 *  the next line
**************************************************/
void ClearDisplay(){
	DisplayClear();
}


/**************************************************
 *  Prints the given number of new line characters
 *  on the screen.
**************************************************/
void PrintNewLine(unsigned int n){
	  for(int i=0; i<n; ++i){
		  printf("\n");
	  }
}

/**************************************************
 *  Prints a --- line on the screen
 *  and sets the number of lines left variable
**************************************************/
void PrintLine(unsigned *N_lineleft){
	for(int i =0; i<N_of_chars_per_line; ++i){
		printf("-");
	}
	printf("\n");
	*N_lineleft-=1;
}

/**************************************************
 * Prints a full line with the given character.
**************************************************/
void PrintFullLineWithChar(unsigned *N_lineleft, char c){
	for(int i =0; i<N_of_chars_per_line; ++i){
			printf("%c",c);
		}
		printf("\n");
		*N_lineleft-=1;
}
