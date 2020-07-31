/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f10x.h"
#include "ADC.h"

custom_libraries::_ADC potentiometer(ADC1,GPIOA,0,custom_libraries::ch0,custom_libraries::SLOW);



int main(void)
{
	potentiometer.initialize();

	//SET ADC NVIC PRIORITY
	NVIC_SetPriority(ADC1_2_IRQn,0x03);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	while(1){


	}
}
