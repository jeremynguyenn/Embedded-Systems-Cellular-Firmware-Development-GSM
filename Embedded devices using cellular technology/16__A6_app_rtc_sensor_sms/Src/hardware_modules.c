#include "hardware_modules.h"
#include "stm32f4xx.h"


#define GPIOCEN		(1U<<2)
#define GPIOAEN		(1U<<0)
#define BTN_PIN		(1U<<13)
#define ADC_CH1		(1U<<0)

#define ADC1EN		(1U<<8)
#define ADC_SEQ_LEN_1	0x00
#define CR2_ADON	(1U<<0)
#define CR2_CONT	(1U<<1)
#define CR2_SWSTART (1U<<30)
#define SR_EOC		(1U<<1)


void light_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;

	/*Set PA5 as output*/
	GPIOA->MODER |=(1U<<10);
	GPIOA->MODER &=~(1U<<11);
}



void light_on(void)
{
	/*Set PA5 output to high*/
	GPIOA->ODR |=(1U<<5);
}


void light_off(void)
{
	/*Set PA5 output to low*/
	GPIOA->ODR &=~(1U<<5);
}



void button_init(void)
{
	/*Enable clock access GPIOC*/
	RCC->AHB1ENR |= GPIOCEN;

	/*Set PC13 to input*/
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);
}


bool get_btn_state(void)
{

	/*Note :  BTN is active low*/

	/*Check if BTN is pressed*/
	if(GPIOC->IDR & BTN_PIN)
	{
		return false;
	}
	else
	{
		return true;
	}

}


void pa1_adc_init(void)
{
	/***Configure the ADC GPIO pin****/
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=GPIOAEN;

	/*Set the mode of PA1 to analog*/
	GPIOA->MODER |=(1U<<2);
	GPIOA->MODER |=(1U<<3);

	/***Configure the ADC module***/
	/*Enable clock access to ADC module*/
	RCC->APB2ENR |=ADC1EN;

	/*Conversion sequence start*/
	ADC1->SQR3 = ADC_CH1;

	/*Conversion sequence length*/
	ADC1->SQR1 = ADC_SEQ_LEN_1;

	/*Enable ADC module*/
	ADC1->CR2 |=CR2_ADON;
}



void start_conversion(void)
{
	/*Enable continuous conversion*/
	ADC1->CR2 |=CR2_CONT;

	/*Start ADC conversion*/
	ADC1->CR2 |=CR2_SWSTART;
}

uint32_t adc_read(void)
{
	/*Wait for conversion to be complete*/
	while(!(ADC1->SR & SR_EOC)){}

	/*Read converted results*/
	return (ADC1->DR);
}
