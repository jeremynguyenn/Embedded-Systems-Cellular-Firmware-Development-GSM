#ifndef __RTC_H__
#define __RTC_H__

#include <stdint.h>


void rtc_alarm_init(void);
void rtc_init(void);
uint32_t rtc_date_get_day(void );
uint32_t rtc_date_get_year(void);
uint32_t rtc_date_get_month(void);
uint32_t rtc_time_get_second(void);
uint32_t rtc_time_get_minute(void);
uint32_t rtc_time_get_hour(void);

uint8_t rtc_convert_bin2bcd(uint8_t value);
uint8_t rtc_convert_bcd2bin(uint8_t value);


#define CR_ALRAIE			(1U<<12)
#define ISR_ALRAF			(1U<<8)
#define ISR_INIT			(1U<<7)

#endif
