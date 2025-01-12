#include "a6_driver.h"


int main()
{
	debug_uart_init();

	while(1)
	{
		printf("A second just occurred!!! \n\r");
		systick_delay_ms(1000);
	}
}
