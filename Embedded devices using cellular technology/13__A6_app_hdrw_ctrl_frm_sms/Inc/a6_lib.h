#ifndef __A6_LIB_H__
#define __A6_LIB_H__

#include "a6_driver.h"
#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define  PHONE_NUM_LEN			13
#define  RAW_SMS_LEN			200
#define  SMS_MSG_LEN			70

void a6_module_init(void);
uint8_t a6_send_sms(char* message, char* phone_number);
uint8_t a6_make_call(char * phone_number);
uint16_t a6_drop_call(void);
uint8_t a6_set_sms_format_to_text(void);
void get_raw_sms(void);
void get_sender_number(void);
void get_sms_msg(void);
uint8_t a6_delete_all_sms(void);

void flush_buffer(void);

uint8_t a6_is_sms(void);


#endif
