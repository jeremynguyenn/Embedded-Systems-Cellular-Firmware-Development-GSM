#include "a6_lib.h"


char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

int main()
{
	a6_module_init();
	//a6_send_sms(sms_message,receiver_number);
	a6_make_call(receiver_number);

}
