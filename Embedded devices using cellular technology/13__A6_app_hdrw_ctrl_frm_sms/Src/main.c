#include "a6_lib.h"
#include "hardware_modules.h"
#include "rtc.h"

#define RTC_BUFF_LEN		30

uint8_t time_buffer[RTC_BUFF_LEN] = {0};
uint8_t date_buffer[RTC_BUFF_LEN] = {0};

void display_rtc_calendar(void);


char receiver_number[] = "7491265055";
char sms_message[]  = "Hello from remote STM32 Machine";


extern char sms_msg[SMS_MSG_LEN];
static void process_cmd(void);
extern uint8_t SMS_NOTIF;
int main()
{

	debug_uart_init();
	a6_uart_init();
	light_init();
	rtc_init();
	rtc_alarm_init();
	a6_module_init();

	a6_set_sms_format_to_text();

	/*Delete all received messages*/
	a6_delete_all_sms();

	a6_send_sms(sms_message,receiver_number);

	while(1)
	{

		for(int i = 0 ; i<10; i++)
		{
			SMS_NOTIF = a6_is_sms();
			systick_delay_ms(1);
		}

		if(SMS_NOTIF)
         {
        	 get_raw_sms();
        	 get_sms_msg();
        	 get_sender_number();

        	 /*Process command*/
        	 process_cmd();

        	 SMS_NOTIF = 0;
         }
	}

}


static void process_cmd(void)
{
	char * ptr1 =  strstr(sms_msg,"LEDON");
	if(ptr1 != NULL)
	{
		light_on();
		flush_buffer();
	}

	char * ptr2 =  strstr(sms_msg,"LEDOFF");
	if(ptr2 != NULL)
	{
		light_off();
		flush_buffer();
	}


}

void display_rtc_calendar(void)
{
  // Display time Format : hh:mm:ss
  sprintf((char*)time_buffer,"%.2d:%.2d:%.2d", rtc_convert_bcd2bin(rtc_time_get_hour()),
		  rtc_convert_bcd2bin(rtc_time_get_minute()),
		  rtc_convert_bcd2bin(rtc_time_get_second()));

  printf(" Time: %.2d:%.2d:%.2d\n\r", rtc_convert_bcd2bin(rtc_time_get_hour()),
		  rtc_convert_bcd2bin(rtc_time_get_minute()),
		  rtc_convert_bcd2bin(rtc_time_get_second()));

  // Display date Format : mm-dd-yy
  sprintf((char*)date_buffer,"%.2d-%.2d-%.2d", rtc_convert_bcd2bin(rtc_date_get_month()),
		  rtc_convert_bcd2bin(rtc_date_get_day()),
          2000 + rtc_convert_bcd2bin(rtc_date_get_year()));


}


void alarm_callback(void)
{

}

void RTC_Alarm_IRQHandler(void)
{
	/*Get the alarm interrupt source enable status*/
	if((RTC->CR & CR_ALRAIE) != 0)
	{
		/*Get the pending status of the Alarm interrupt*/
		if((RTC->ISR & ISR_ALRAF) != 0)
		{
			/*Do something...*/
			alarm_callback();

			/*Clear the alarm interrupt pending bit*/
			RTC->ISR &=~(ISR_INIT);
		}
	}

	/*Clear the EXTI's flag for RTC alarm*/
	EXTI->PR = (1U<<17);
}
