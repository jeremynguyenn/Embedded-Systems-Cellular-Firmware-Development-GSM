#include "a6_lib.h"


char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";

int main()
{
	a6_module_init();
	a6_make_call(receiver_number);
	systick_delay_ms(25000); /*Wait a bit*/
	a6_drop_call();  /*Then drop call*/

}
