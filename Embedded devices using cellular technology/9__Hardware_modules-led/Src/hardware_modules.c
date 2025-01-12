#include "hardware_modules.h"
#include "stm32f4xx.h"



void light_init(void)
{
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR |=(1U<<0);

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
