#include "a6_driver.h"


#define GPIOAEN			(1U<<0) // 0B 0000 0000 0000 0000 0000 0000 0000 0001
#define UART2EN			(1U<<17)
#define UART1EN			(1U<<4)

#define CR1_UE			(1U<<13)
#define CR1_RE			(1U<<2)
#define CR1_TE			(1U<<3)
#define SR_TXE			(1U<<7)

#define CR1_RXNEIE		(1U<<5)

#define UART_BAUDRATE		115200
#define SYS_FREQ			16000000

static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);
static void debug_uart_write(int ch);

int __io_putchar(int ch)
{
	debug_uart_write(ch);
	return ch;
}

void debug_uart_init(void)
{
	/*1. Enable clock access to UART pins' GPIO port (GPIOA)*/
	RCC->AHB1ENR |= GPIOAEN;

	/*2. Set PA2 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->MODER |= (1U<<5);

	/*3. Set PA3 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<6);
	GPIOA->MODER |= (1U<<7);

	/*4.Set PA2 alternate function type to UART2_TX (AF07)*/
	GPIOA->AFR[0] |=(1U<<8);
	GPIOA->AFR[0] |=(1U<<9);
	GPIOA->AFR[0] |=(1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	/*4.Set PA3 alternate function type to UART2_RX (AF07)*/
	GPIOA->AFR[0] |=(1U<<12);
	GPIOA->AFR[0] |=(1U<<13);
	GPIOA->AFR[0] |=(1U<<14);
	GPIOA->AFR[0] &= ~(1U<<15);

	/*Configure UART module*/

	/*5. Enable clock access to the UART module*/
	RCC->APB1ENR |= UART2EN;

	/*6. Disable UART module*/
	USART2->CR1 &=~CR1_UE;

	/*7. Set UART baudrate*/
	USART2->BRR =  compute_uart_bd(SYS_FREQ,UART_BAUDRATE);

    /*8. Set trnasfer direction*/
	USART2->CR1 = (CR1_TE | CR1_RE);

	/*9. Enable UART module*/
	USART2->CR1 |= CR1_UE;


}

/*
 * UART Module :  USART1
 * UART Pins   :  PA9 = TX, PA10 =RX
 *
 * A6 TX Pin   ==> PA10(RX)
 * A6 RX Pin   ==> PA9(TX)
 * */

void a6_uart_init(void)
{
	/*1. Enable clock access to UART pins' GPIO port (GPIOA)*/
	RCC->AHB1ENR |= GPIOAEN;

	/*2. Set PA9 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<18);
	GPIOA->MODER |= (1U<<19);

	/*3. Set PA10 mode to alternate function*/
	GPIOA->MODER &= ~(1U<<20);
	GPIOA->MODER |= (1U<<21);

	/*4.Set PA9 alternate function type to UART1_TX (AF07)*/
	GPIOA->AFR[1] |=(1U<<4);
	GPIOA->AFR[1] |=(1U<<5);
	GPIOA->AFR[1] |=(1U<<6);
	GPIOA->AFR[1] &= ~(1U<<7);

	/*4.Set PA10 alternate function type to UART1_RX (AF07)*/
	GPIOA->AFR[1] |=(1U<<8);
	GPIOA->AFR[1] |=(1U<<9);
	GPIOA->AFR[1] |=(1U<<10);
	GPIOA->AFR[1] &= ~(1U<<11);

	/*Configure UART module*/

    /*5. Enable clock access to the UART module*/
     RCC->APB2ENR |= UART1EN;

 	/*6. Disable UART module*/
 	 USART1->CR1 &=~CR1_UE;

 	/*7. Set UART baudrate*/
 	USART1->BRR =  compute_uart_bd(SYS_FREQ,UART_BAUDRATE);

    /*8. Set trnasfer direction*/
	USART1->CR1 = (CR1_TE | CR1_RE);

	/*9. Enable RXNEIE/*/

	USART1->CR1 |= CR1_RXNEIE;


 	/*6. Disable UART module*/
 	USART1->CR1 |= CR1_UE;


}

void a6_uart_write_char(char ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART1->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART1->DR = (ch & 0xFF);

}

static void debug_uart_write(int ch)
{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & SR_TXE)){}

	/*Write to transmit data register*/
	USART2->DR = (ch & 0xFF);
}

static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
	return ((periph_clk + (baudrate/2U))/baudrate);
}
