#ifndef __A6_LIB_H__
#define __A6_LIB_H__

#include "a6_driver.h"
#include "fifo.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void a6_module_init(void);
uint8_t a6_send_sms(char* message, char* phone_number);
uint8_t a6_make_call(char * phone_number);

#endif
