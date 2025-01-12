#include "a6_lib.h"
#include "hardware_modules.h"

char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

bool btn_state;

int main()
{
	light_init();
	button_init();
	//a6_module_init();

	while(1)
	{
		btn_state = get_btn_state();
	}

}
