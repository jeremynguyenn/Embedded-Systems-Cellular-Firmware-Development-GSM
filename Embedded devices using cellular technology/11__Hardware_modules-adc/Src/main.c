#include "a6_lib.h"
#include "hardware_modules.h"

char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

bool btn_state;
uint32_t sensor_value;
int main()
{
	light_init();
	button_init();
	pa1_adc_init();
	start_conversion();

	while(1)
	{
		sensor_value = adc_read();
	}

}
