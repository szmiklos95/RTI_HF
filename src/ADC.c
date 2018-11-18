/*
 * ADC.c
 *
 *  Created on: 2017. márc. 31.
 *      Author: MEMS
 */

#include "ADC.h"


/**************************************************
 *  Initializes the ADC module
**************************************************/
void InitADC(){
	CMU_ClockEnable(cmuClock_ADC0, true);

	ADC_Init_TypeDef init = ADC_INIT_DEFAULT;

	init.ovsRateSel = adcOvsRateSel2;
	init.lpfMode = adcLPFilterBypass;
	init.warmUpMode = adcWarmupNormal;
	init.timebase = ADC_TimebaseCalc(0);
	init.prescale = ADC_PrescaleCalc(7000000, 0);
	init.tailgate = 0;

	ADC_Init(ADC0, &init);
}


/**************************************************
 *  Makes an ADC scan on CH4 = PD4 and returns the value
**************************************************/
uint32_t GetADCvalue(void) {

	ADC_InitScan_TypeDef scanInit = ADC_INITSCAN_DEFAULT;
	//12 bit resolution and VDD reference means the range from 0 to 3.3V gets diveded to 2^12 = 4096 points
	// In total darkness the photodiode doesn't let any current through so the full 3.3V is on the photodiode meaning the ADC value ~= 4096
	//More light means less ADC value

	scanInit.reference = adcRefVDD;
	scanInit.input     = ADC_SCANCTRL_INPUTMASK_CH4;

	ADC_InitScan(ADC0, &scanInit);

	ADC_Start(ADC0, adcStartScan);

	while (ADC0->STATUS & ADC_STATUS_SCANACT) ;
	//Get ADC result
	return ADC_DataScanGet(ADC0);

}


/**************************************************
 *  Displays the ADC value
**************************************************/
void DisplayADCvalue(unsigned *N_lineleft){
	printf("Light:%lu\n", 4096-GetADCvalue());
	*N_lineleft-=1;
}

/**************************************************
 *  Fills a line with the chosen character.
 *  The more characters means the more ambient light.
**************************************************/
void DrawADCvalue(unsigned *N_lineleft, char c){
	uint32_t ADCvalue = 4096-GetADCvalue(); //where 4096 comes from the 12 bit ADC resolution settings
	uint32_t compvalue = 4096 / N_of_chars_per_line;
	for(int i = 0; i< N_of_chars_per_line; ++i){
		if(ADCvalue > compvalue*i){
			printf("%c",c);
		}
	}
	printf("\n");
	*N_lineleft-=1;
}


