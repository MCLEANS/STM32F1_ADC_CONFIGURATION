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

uint16_t potentiometer_value = 0;

custom_libraries::_ADC potentiometer(ADC1,GPIOA,0,custom_libraries::ch0,custom_libraries::SLOW);

extern "C" void ADC1_2_IRQHnadler(void){
	//CHECK THAT WE ARE HERE BECAUSE OF ADC1 EOC
	if(ADC1->SR & ADC_SR_EOC){
		//CLEAR THE FLAG
		ADC1->SR &= ~ADC_SR_EOC;
		potentiometer_value = ADC1->DR;
		//START THE NEXT CONVERSION
		ADC1->CR2 |= ADC_CR2_SWSTART;
	}
}

int main(void)
{
	potentiometer.initialize();

	//SET ADC NVIC PRIORITY
	NVIC_SetPriority(ADC1_2_IRQn,0x03);
	NVIC_EnableIRQ(ADC1_2_IRQn);

	while(1){


	}
}
