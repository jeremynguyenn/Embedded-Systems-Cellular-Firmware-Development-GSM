#ifndef __HARDWARE_MODULES_H__
#define __HARDWARE_MODULES_H__
#include <stdbool.h>


void light_init(void);
void light_on(void);
void light_off(void);

void button_init(void);
bool get_btn_state(void);

#endif
