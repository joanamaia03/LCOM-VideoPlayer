#ifndef _KEYBOARD_H
#define _KEYBOARD_H
#include <lcom/lcf.h>
#include "i8042.h"
int(kbc_subscribe_int)(int *bit_no);
int(kbc_unsubscribe_int)(int hook_id);
void(kbc_ih)();
int(kbd_enable)();
#endif 
