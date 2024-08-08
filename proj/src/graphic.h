///Provides functions that are related to the graphics manipulation in VBE mode.
///

#ifndef _LCOM_GRAPHIC_H_
#define _LCOM_GRAPHIC_H_

#include <minix/sysutil.h>
#include <machine/int86.h>
#include <lcom/lcf.h>
#include <lcom/lab5.h>
#include <lcom/xpm.h>
#include <lcom/pixmap.h>

///The function graphic_set_VBE_mode(uint16_t mode) is responsible for setting the specified VBE mode.
///
int(graphic_set_VBE_mode)(uint16_t mode);
///The function changePixelColor(uint16_t x, uint16_t y, uint32_t color) changes the color of a pixel at position (x, y) in the framebuffer.
///
int(changePixelColor)(uint16_t x,uint16_t y,uint32_t color);
///The function vg_draw_hline(uint16_t x,uint16_t y,uint16_t len,uint32_t color) draws a horizontal line in the framebuffer.
///
int(vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len,uint32_t color);
///The function graphic_draw_rectangle(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color) draws a rectangle in the framebuffer.
///
int(graphic_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color);
///The function xmp_draw(xpm_image_t img, xpm_map_t xpm, uint16_t x, uint16_t y) draws an XPM image in the framebuffer.
///
int(xmp_draw)(xpm_image_t img,xpm_map_t xpm, uint16_t x, uint16_t y);
/// This function calculates the value of the red component of a color, based on an initial value first, an index j and a step. 
///
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first);
/// This function calculates the value of the green component of a color, based on an initial value first, an index i and a step. 
///
uint32_t (Green)(unsigned j, uint8_t step, uint32_t first);
/// This function calculates the value of the blue component of a color, based on an initial value first, indices i and j, and a step..
///
uint32_t (Blue)(unsigned j,unsigned i, uint8_t step, uint32_t first);
/// This function draws a frame that is exported from the video codec
///
int(draw_frame)(uint16_t x, uint16_t y, int width, int height, unsigned char* buffer);
///This function extracts the red component of a color, based on the first value.
///
uint32_t (R)(uint32_t first);
/// This function extracts the blue component of a color, based on the first value.  
///
uint32_t (B)(uint32_t first);
/// This function extracts the green component of a color, based on the first value.
///
uint32_t (G)(uint32_t first);
/// This function combines the values of the red, green and blue components to form one color in spot color mode.
///
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);
///This function calculates a color value for the indexed color mode, based on the coordinates col and row, a step step, a start value first and the number of colors n.
///
uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n);
///The function xmp_draw_char(xpm_image_t img, xpm_map_t xpm, uint16_t x, uint16_t y, char a) draws a specific character from an XPM font in the framebuffer.
///
int(xmp_draw_char)(xpm_image_t img,xpm_map_t xpm, uint16_t x, uint16_t y,char a);
///The function read_frame() is responsible for passing the frames that come from the codec.
///
void (readFrame)(int* Pwidth, int* Pheight, char* buff2);

#endif

