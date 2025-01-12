#include "a6_lib.h"
#include "hardware_modules.h"

char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

int main()
{
	light_init();
	//a6_module_init();

	while(1)
	{
		light_on();
	}

}
