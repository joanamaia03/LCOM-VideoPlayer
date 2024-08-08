///Provides functions that display the menu and  interact with the keyboard.
///


#ifndef _MENU_H
#define _MENU_H
#include <lcom/lcf.h>

#define GREY_COLOR 0x808080
///The function ih_enable() is responsible for initializing the GUI and subscribing to keyboard interrupts.
///
void(ih_enable)();
///The function init_draw() is responsible for drawing the initial menu.
///
void(init_draw)();
///The draw_video_menu() function is responsible for drawing the video menu.
///
void(draw_video_menu)();
#endif

