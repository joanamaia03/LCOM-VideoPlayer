///Provides functions that control different functionalities.
///




#ifndef _KBD_H_
#define _KBD_H_

#include <lcom/lcf.h>
#include "i8042.h"

#define DELAY_US 20000
///The kbc_subscribe_int(uint8_t *bit_no) function is responsible for subscribing to keyboard interrupts.
///
int(kbc_subscribe_int)(uint8_t *bit_no);
///The kbc_unsubscribe_int() function is responsible for unsubscribing from keyboard interrupts.
///
int(kbc_unsubscribe_int)();
///The kbc_ih() function is the keyboard interrupt service routine.
///
void(kbc_ih)();
///The function kbd_enable() is responsible for enabling the keyboard.
///
int(kbd_enable)();
///The function kbd_scan_esc() is responsible for scanning the keyboard until the "ESC" key is pressed.
///
int(kbd_scan_esc)();
///The function kbd_scan_space() is responsible for scanning the keyboard until the space key is pressed.
///
int(kbd_scan_space)();

#endif 
