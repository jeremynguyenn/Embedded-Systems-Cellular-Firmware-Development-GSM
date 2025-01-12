#include "a6_lib.h"


char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

int main()
{
	a6_module_init();
	a6_set_sms_format_to_text();

}
