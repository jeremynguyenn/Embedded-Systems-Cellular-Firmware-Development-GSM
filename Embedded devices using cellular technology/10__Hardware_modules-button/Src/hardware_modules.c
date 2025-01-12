#include "hardware_modules.h"
#include "stm32f4xx.h"


#define GPIOCEN		(1U<<2)
#define GPIOAEN		(1U<<0)
#define BTN_PIN		(1U<<13)

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

