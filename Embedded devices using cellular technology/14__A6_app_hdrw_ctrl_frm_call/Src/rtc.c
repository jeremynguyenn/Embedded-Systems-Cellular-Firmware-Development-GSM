#include "rtc.h"
#include "stm32f4xx.h"


#define PWREN			(1U<<28)
#define CR_DBP			(1U<<8)
#define CSR_LSION		(1U<<0)
#define CSR_LSIRDY		(1U<<1)
#define BDCR_BDRST		(1U<<16)
#define RTCEN			(1U<<15)
#define CR_FMT			(1U<<6)
#define ISR_INIT		(1U<<7)
#define ISR_RSF 		(1U<<5)


#define RTC_WRITE_PROTECTION_ENABLE_1 ((uint8_t)0xCAU)
#define RTC_WRITE_PROTECTION_ENABLE_2 ((uint8_t)0x53U)

#define RTC_INIT_MASK		0xFFFFFFFFU

#define RTC_ASYNCH_PREDIV          ((uint32_t)0x7F)
#define RTC_SYNCH_PREDIV           ((uint32_t)0x00F9)

#define MONTH_JANUARY             		((uint8_t)0x01U)
#define WEEKDAY_FRIDAY             		((uint8_t)0x05U)
#define TIME_FORMAT_PM             		(1U<<22)
#define ALMA_MASK_DATEWEEKDAY       	RTC_ALRMAR_MSK4
#define CR_ALRAE						(1U<<8)
#define ISR_ALRAF						(1U<<8)

static uint32_t rtc_init_seq(void);
static void rtc_set_async_prescaler(uint32_t AsyncPrescaler);
static void rtc_set_synch_prescaler(uint32_t SynchPrescaler);
static uint32_t exit_init_seq(void);
static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year);
static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds);
static void rtc_alma_config_time(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds);
static void rtc_alma_set_mask(uint32_t Mask);

void rtc_init(void)
{
	/*Enable clock access to PWR*/
	RCC->APB1ENR |= PWREN;

	/*Enable Backup access to config rtc*/
	PWR->CR |= CR_DBP;

	/*Enable Low Speed Internal (LSI) clock*/
	RCC->CSR |=CSR_LSION;

	/*Wait for LSI to be enabled*/
	while((RCC->CSR & CSR_LSIRDY) != CSR_LSIRDY){}

	/*Force backup domain to reset*/
	RCC->BDCR |=BDCR_BDRST;

	/*Release backup domain reset*/
	RCC->BDCR &=~BDCR_BDRST;

	/*Set RTC clock source to LSI*/
	RCC->BDCR &=~(1U<<8);
	RCC->BDCR |=(1U<<9);

	/*Enable RTC*/
	RCC->BDCR |= RTCEN;

	/*Disable RTC registers write protection*/
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_1;
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_2;


	/*Enter initialization*/
	if(rtc_init_seq() != 1)
	{
		/*Initialization error*/
	}

	/*Set hour format to AM/PM*/
	RTC->CR |= CR_FMT;

	/*Set Asynch prescaler*/
	rtc_set_async_prescaler(RTC_ASYNCH_PREDIV);

	/*Set Synch prescaler*/
	rtc_set_synch_prescaler(RTC_SYNCH_PREDIV);

	/*Exit intialization mode*/
	exit_init_seq();

	/*Enable RTC registers write protection*/
	RTC->WPR = 0xFF;
}

void rtc_alarm_init(void)
{
	/*Disable RTC registers write protection*/
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_1;
	RTC->WPR = RTC_WRITE_PROTECTION_ENABLE_2;

	/*Enter initialization*/
	if(rtc_init_seq() != 1)
	{
		/*Initialization error*/
	}

    /*Set date : Friday 14th, January 2022*/
	rtc_date_config(WEEKDAY_FRIDAY,0x14,MONTH_JANUARY,0x22);

	/*Set time : 07:04: 00 PM*/
	rtc_time_config(TIME_FORMAT_PM,0x07,0x04,0x00);

	/*Set alarm :  07:04:10*/

	/*RTC Alarm Generation: Alarm on Hours, Minutes and Seconds*/
	rtc_alma_config_time(TIME_FORMAT_PM,0x07,0x04,0x10);

    /*Ignore weekday*/
	rtc_alma_set_mask(ALMA_MASK_DATEWEEKDAY);

	/*Enable alarm*/
	RTC->CR |=CR_ALRAE;

	/*Enable IT Alarm*/
	RTC->CR |= CR_ALRAIE;

	/*Clear the alarm interrupt pending bit*/
	RTC->ISR &=~ISR_ALRAF;


	/*Enable IT alarm*/
	EXTI->IMR |=(1U<<17);
	EXTI->RTSR |=(1U<<17);

	/*Configure RTC alarm interrupt*/

	/*Enable rtc alarm interrupt in NVIC*/
	NVIC_EnableIRQ(RTC_Alarm_IRQn);


	/*Exit intialization mode*/
	RTC->ISR &=~ISR_INIT;

	/*Clear RSF flag*/
	RTC->ISR &=~ISR_RSF;

	if(	exit_init_seq() != 1)
	{
		//process error
	}
	/*Enable RTC registers write protection*/
	RTC->WPR = 0xFF;


}


static void rtc_alma_set_mask(uint32_t Mask)
{
  MODIFY_REG(RTC->ALRMAR, RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2 | RTC_ALRMAR_MSK1, Mask);
}


static void rtc_date_config(uint32_t WeekDay, uint32_t Day, uint32_t Month, uint32_t Year)
{
  register uint32_t temp = 0U;

  temp = (WeekDay << RTC_DR_WDU_Pos)                                                        | \
         (((Year & 0xF0U) << (RTC_DR_YT_Pos - 4U)) | ((Year & 0x0FU) << RTC_DR_YU_Pos))   | \
         (((Month & 0xF0U) << (RTC_DR_MT_Pos - 4U)) | ((Month & 0x0FU) << RTC_DR_MU_Pos)) | \
         (((Day & 0xF0U) << (RTC_DR_DT_Pos - 4U)) | ((Day & 0x0FU) << RTC_DR_DU_Pos));

  MODIFY_REG(RTC->DR, (RTC_DR_WDU | RTC_DR_MT | RTC_DR_MU | RTC_DR_DT | RTC_DR_DU | RTC_DR_YT | RTC_DR_YU), temp);
}



static void rtc_time_config(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds)
{
  register uint32_t temp = 0U;

  temp = Format12_24                                                                                    | \
         (((Hours & 0xF0U) << (RTC_TR_HT_Pos - 4U)) | ((Hours & 0x0FU) << RTC_TR_HU_Pos))     | \
         (((Minutes & 0xF0U) << (RTC_TR_MNT_Pos - 4U)) | ((Minutes & 0x0FU) << RTC_TR_MNU_Pos)) | \
         (((Seconds & 0xF0U) << (RTC_TR_ST_Pos - 4U)) | ((Seconds & 0x0FU) << RTC_TR_SU_Pos));
  MODIFY_REG(RTC->TR, (
		  RTC_TR_PM | RTC_TR_HT | RTC_TR_HU | RTC_TR_MNT | RTC_TR_MNU | RTC_TR_ST | RTC_TR_SU), temp);
}



static void rtc_alma_config_time(uint32_t Format12_24, uint32_t Hours, uint32_t Minutes, uint32_t Seconds)
{
  register uint32_t temp = 0U;

  temp = Format12_24 | (((Hours & 0xF0U) << (RTC_ALRMAR_HT_Pos - 4U)) | ((Hours & 0x0FU) << RTC_ALRMAR_HU_Pos))    | \
         (((Minutes & 0xF0U) << (RTC_ALRMAR_MNT_Pos - 4U)) | ((Minutes & 0x0FU) << RTC_ALRMAR_MNU_Pos)) | \
         (((Seconds & 0xF0U) << (RTC_ALRMAR_ST_Pos - 4U)) | ((Seconds & 0x0FU) << RTC_ALRMAR_SU_Pos));

  MODIFY_REG(RTC->ALRMAR, RTC_ALRMAR_PM | RTC_ALRMAR_HT | RTC_ALRMAR_HU | RTC_ALRMAR_MNT | RTC_ALRMAR_MNU | RTC_ALRMAR_ST | RTC_ALRMAR_SU, temp);
}

static void rtc_set_async_prescaler(uint32_t AsynchPrescaler)
{

	MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_A, AsynchPrescaler << RTC_PRER_PREDIV_A_Pos);

}

static void rtc_set_synch_prescaler(uint32_t SynchPrescaler)
{
  MODIFY_REG(RTC->PRER, RTC_PRER_PREDIV_S, SynchPrescaler);
}

static void rtc_enable_init_mode(void)
{
	RTC->ISR = RTC_INIT_MASK;
}

static uint32_t rtc_isActiveflag_init(void)
{
	return (READ_BIT(RTC->ISR,RTC_ISR_INITF) ==  (RTC_ISR_INITF));
}

static uint32_t rtc_init_seq(void)
{
	/*Set initialization mode*/
	rtc_enable_init_mode();

	/*Check if initialization mode is set*/
	while(rtc_isActiveflag_init() != 1){}

	return 1;
}



static void rtc_exit_init_mode(void)
{
	RTC->ISR = 0;

}


void rtc_clearflag_rs(void)
{
    WRITE_REG(RTC->ISR, (~((RTC_ISR_RSF | RTC_ISR_INIT) & 0x0000FFFFU) | (RTC->ISR & RTC_ISR_INIT)));

}

static uint32_t  rtc_isActiveflag_rs(void)
{
    return (READ_BIT(RTC->ISR, RTC_ISR_RSF) == (RTC_ISR_RSF));

}
static uint32_t wait_for_synchro(void)
{
	/*Clear RSF flag*/
	rtc_clearflag_rs();

	/*Wait for registers to be synched*/
	while(rtc_isActiveflag_rs() != 1)
	{

	}

	return 0;
}

static uint32_t exit_init_seq(void)
{
	/*Disable rtc init mode*/
	rtc_exit_init_mode();

	/*Wait for registers to be synchronised*/
	return (wait_for_synchro());

}


uint8_t rtc_convert_bin2bcd(uint8_t value)
{
	return  (uint8_t)((((value) / 10U) << 4U) | ((value) % 10U));
}

uint8_t rtc_convert_bcd2bin(uint8_t value )
{
	return (uint8_t)(((uint8_t)((value) & (uint8_t)0xF0U) >> (uint8_t)0x4U) * 10U + ((value) & (uint8_t)0x0FU));
}



uint32_t rtc_date_get_day (void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_DT | RTC_DR_DU))) >> RTC_DR_DU_Pos);
}

uint32_t rtc_date_get_year(void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_YT | RTC_DR_YU))) >> RTC_DR_YU_Pos);
}


uint32_t rtc_date_get_month(void)
{
 return (uint32_t)((READ_BIT(RTC->DR, (RTC_DR_MT | RTC_DR_MU)))>> RTC_DR_MU_Pos);
}

uint32_t rtc_time_get_second(void)
{
 return (uint32_t)(READ_BIT(RTC->TR, (RTC_TR_ST | RTC_TR_SU)) >> RTC_TR_SU_Pos);
}

uint32_t rtc_time_get_minute(void)
{
  return (uint32_t)(READ_BIT(RTC->TR, (RTC_TR_MNT | RTC_TR_MNU))>> RTC_TR_MNU_Pos);
}


uint32_t rtc_time_get_hour(void)
{
 return (uint32_t)((READ_BIT(RTC->TR, (RTC_TR_HT | RTC_TR_HU))) >> RTC_TR_HU_Pos);
}

