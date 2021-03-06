/*
 * ADC.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: MCLEANS
 */

#include "ADC.h"

namespace custom_libraries {

_ADC::_ADC(ADC_TypeDef *ADC_,
			GPIO_TypeDef *GPIO,
			uint8_t PIN,
			ADC_channel channel,
			Sampling_rate sample_rate):ADC_(ADC_),
										GPIO(GPIO),
										PIN(PIN),
										channel(channel),
										sample_rate(sample_rate){

	//Enable GPIO_RCC
	if(GPIO == GPIOA) RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	if(GPIO == GPIOB) RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	if(GPIO == GPIOC) RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	if(GPIO == GPIOD) RCC->APB2ENR |= RCC_APB2ENR_IOPDEN;
	if(GPIO == GPIOE) RCC->APB2ENR |= RCC_APB2ENR_IOPEEN;

	//Enable ADC_RCC
	if(ADC_ == ADC1) RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	if(ADC_ == ADC2) RCC->APB2ENR |= RCC_APB2ENR_ADC2EN;

	//Set GPIO to analog mode
	if(PIN < 8){
		//SET PIN TO INPUT
		GPIO->CRL &= ~(1 << (PIN*4));
		GPIO->CRL &= ~(1 << ((PIN*4)+1));
		//SET TO ANALOG MODE
		GPIO->CRL &= ~(1 << ((PIN*4)+2));
		GPIO->CRL &= ~(1 << (((PIN*4)+2)+1));
	}
	else{
		//SET PIN TO INPUT
		GPIO->CRH &= ~(1 << ((PIN-8)*4));
		GPIO->CRH &= ~(1 << (((PIN-8)*4)+1));
		//SET TO ANALOG MODE
		GPIO->CRH &= ~(1 << (((PIN-8)*4)+2));
		GPIO->CRH &= ~(1 << ((((PIN-8)*4)+2)+1));
	}

	//Set ADC prescaler
	RCC->CFGR |= RCC_CFGR_ADCPRE_1; //prescaler of 6 to set clock to 12MHz (must be < 14MHz)

	//enable ADC end of conversion
	ADC_->CR1 |= ADC_CR1_EOCIE;

	//ENABLE TRIGGER SOURCE
	ADC_->CR2 |= ADC_CR2_EXTTRIG;

	//SELECT ACTUAL CONVERSION TRIGGER
	ADC_->CR2 |= ADC_CR2_EXTSEL;

	//Set the sampling rate
	switch(sample_rate){
		case SLOW:
			if(channel < 10){
				ADC_->SMPR2 |= (1 <<((channel*3)));
				ADC_->SMPR2 |= (1 <<((channel*3)+1));
				ADC_->SMPR2 |= (1 <<((channel*3)+2));
			}
			else{
				ADC_->SMPR1 |= (1 <<(((channel-10)*3)));
				ADC_->SMPR1 |= (1 <<(((channel-10)*3)+1));
				ADC_->SMPR1 |= (1 <<(((channel-10)*3)+2));
			}
			break;
		case MEDIUM:
			if(channel < 10){
				ADC_->SMPR2 &= ~(1 <<((channel*3)));
				ADC_->SMPR2 &= ~(1 <<((channel*3)+1));
				ADC_->SMPR2 |= (1 <<((channel*3)+2));
			}
			else{
				ADC_->SMPR1 &= ~(1 <<(((channel-10)*3)));
				ADC_->SMPR1 &= ~(1 <<(((channel-10)*3)+1));
				ADC_->SMPR1 |= (1 <<(((channel-10)*3)+2));
			}
			break;
		case FAST:
			if(channel < 10){
				ADC_->SMPR2 &= ~(1 <<((channel*3)));
				ADC_->SMPR2 &= ~(1 <<((channel*3)+1));
				ADC_->SMPR2 &= ~(1 <<((channel*3)+2));
			}
			else{
				ADC_->SMPR1 &= ~(1 <<(((channel-10)*3)));
				ADC_->SMPR1 &= ~(1 <<(((channel-10)*3)+1));
				ADC_->SMPR1 &= ~(1 <<(((channel-10)*3)+2));
			}
			break;
		default:
			break;

	}


	//Set number of channels
	//Set all its registers to zero signaling one conversion
	ADC_->SQR1 &= ~ADC_SQR1_L;

	//Set channel sequence
	switch(channel){
		case ch0:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch1:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch2:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch3:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch4:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch5:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch6:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch7:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch8:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch9:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch10:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch11:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch12:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch13:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch14:
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		case ch15:
			ADC_->SQR3 |= ADC_SQR3_SQ1_0;
			ADC_->SQR3 |= ADC_SQR3_SQ1_1;
			ADC_->SQR3 |= ADC_SQR3_SQ1_2;
			ADC_->SQR3 |= ADC_SQR3_SQ1_3;
			ADC_->SQR3 &= ~ADC_SQR3_SQ1_4;
			break;
		default:
			break;
	}
}

void _ADC::delay(){
	for(int i=0; i<9000000;i++){};
}

void _ADC::initialize(){
	//Enable ADC and set to continuous mode
	//This first enabling wakes it up from sleep
	ADC_->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;

	//delay time Tstab as stated in reference manual
	delay();

	//Enable ADC again to start
	//The second enable actually enables the ADC
	ADC_->CR2 |= ADC_CR2_ADON;

	delay();

	//CALIBRATE THE ADC
	ADC_->CR2 |= ADC_CR2_CAL;
	//WAIT FOR ADC TO CALIBRATE
	while(ADC_->CR2 & ADC_CR2_CAL){}
	//start first ADC conversion
	ADC_->CR2 |= ADC_CR2_SWSTART;
}

_ADC::~_ADC() {
	// TODO Auto-generated destructor stub
}

} /* namespace custom_libraries */
