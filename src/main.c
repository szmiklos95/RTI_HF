#include "em_device.h"
#include "em_chip.h"

#include "SPI.h"
#include "SysTick.h"
#include "ADXL362.h"
#include "GPIO.h"
#include "UART_Debugger.h"
#include "LCD.h"
#include "SI7021.h"
#include "ADC.h"

#include "EnergyModes.h"


/*
 * State of coding:
 * - Accelerometer data seems to be good, but temperature data needs to be calibrated
 * - I should build in a check for the sensor ID (for both sensors).
 * 		If the ID received doesn't match the ID in the datasheet, then display something like "Invalid" or "xxx".
 * - Display functions should check if you have enough lines left
 * - I should add a prohibited gpio list. Example: If the humitemp sensor active it uses 2 pins for I2C.
 * 		I could use macros to activate and deactivate modules like the humitemp sensor
 * - Make doxygen comments
 * - Connect VBAK_OK
 */

int main(void)
{

  CHIP_Init();

  InitDisplay();
  InitGPIO();

  InitSysTick();
  InitUSART();
  InitADXL362();
  //InitDebugger();
  InitSI7021();
  InitADC();

  printf("\nADXL362 ID: 0x%x\n", ReadRegister(DEVICE_ID_REG));
  //printf("ADXL362 Part ID: 0x%x\n", ReadRegister(PART_ID_REG));
  PrintNewLine(11);

  unsigned int N_lineleft; //The number of lines left on the screen

  while (1) {


	  if(StartMeasurement()){

		  //ClearDisplay();
		  N_lineleft = 16;

		  DisplayXYZTData(&N_lineleft);
		  printf("ADXL362 ID: 0x%x", ReadRegister(DEVICE_ID_REG)); N_lineleft-=1;

		  PrintFullLineWithChar(&N_lineleft, '-');
		  DisplayHumTempData(&N_lineleft);

		  PrintFullLineWithChar(&N_lineleft, '-');
		  DisplayADCvalue(&N_lineleft);
		  DrawADCvalue(&N_lineleft, '#');

		  PrintNewLine(N_lineleft);

		  Delay(1);
	  }
	  else{
		  em_EM3_Ulfrco();
	  }
  }
}
