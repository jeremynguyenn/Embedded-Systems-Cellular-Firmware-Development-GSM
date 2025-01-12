#include "a6_lib.h"
#include "hardware_modules.h"
#include "rtc.h"

#define RTC_BUFF_LEN		30

uint8_t time_buffer[RTC_BUFF_LEN] = {0};
uint8_t date_buffer[RTC_BUFF_LEN] = {0};

void display_rtc_calendar(void);

int main()
{

	debug_uart_init();
	rtc_init();
	rtc_alarm_init();
	while(1)
	{
		display_rtc_calendar();
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
	printf("ALARM JUST OCCURRED !!\n\r");
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

