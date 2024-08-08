// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>
#include <lcom/pp.h>
#include <stdint.h>
/*
1º - Configure the video card to operate in the mode specified in its
first argument
2º map its frame-buffer
3º Then draw a line (either horizontal or vertical)
4º Sleep for delay seconds
5º Switch back to Minix’s text mode
*/
// Any header files included below this line should have been created by you
int main(int argc, char *argv[]) {
// sets the language of LCF messages (can be either EN-US or PT-PT)
lcf_set_language("EN-US");
// enables to log function invocations that are being "wrapped" by LCF
// [comment this out if you don't want/need it]
lcf_trace_calls("/home/lcom/labs/pp/trace.txt");
// enables to save the output of printf function calls on a file
// [comment this out if you don't want/need it]
lcf_log_output("/home/lcom/labs/pp/output.txt");
// handles control over to LCF
// [LCF handles command line arguments and invokes the right function]
if (lcf_start(argc, argv))
return 1;
// LCF clean up tasks
// [must be the last statement before return]
lcf_cleanup();
return 0;
}
typedef struct {
uint16_t x_res; // horizontal resolution
uint16_t y_res; // vertical resolution
uint8_t bpp; // bits per pixel
uint8_t r_sz; // red component size
uint8_t r_pos; // red component LSB position
uint8_t g_sz; // green component size
uint8_t g_pos; // green component LSB position
uint8_t b_sz; // blue component size
uint8_t b_pos; // blue component LSB position
phys_addr_t phys_addr; // video ram base phys address
} lpv_mode_info_t;

#define BIT (n)(1<<(n))
#define Text_Mode 0
#define Indexed 1
#define Direct_Colour 2
#define CtrlReg 0x24
#define EnableSVRInt 7

static char* vram;
static uint8_t bytes_per_pixel;
static uint32_t vram_size;
static lpv_mode_info_t screen_res;

int lpv_mode_select(uint8_t mode){
    uint8_t command = 0x00
    if(mode == 0){
        command = 0;
    }
    else if(mode == 1){
        command = 1;
    }
    else if(mode == 2){
        command = 2;
    }

    if(sys_outb(CtrlReg, command) != 0){
        panic("Fatal ERROR shutting down...");
        return 1;
    }
    return 0;
}

int drawPixel(uint16_t x, uint16_t y, uint32_t colour){
    if(x > screen_res.x_res || x < 0){
        panic("ERROR pixel out of screen");
        return 1;
    }
    if(y > screen_res.y_res || y < 0){
        panic("ERROR pixel out of screen");
        return 1;
    }
    uint32_t pos = (y*screen_res.x_res + x) * bytes_per_pixel;
    memcpy(vram + pos, &colour, bytes_per_pixel);

    return 0;
}

int drawLineH(uint16_t x, uint16_t y, uint32_t colour, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        if(drawPixel(x+i, y, colour) != 0){
            panic("ERROR could not draw pixel")
            return 1;
        }
    }
    return 0;
}
int drawLineV(uint16_t x, uint16_t y, uint32_t colour, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        if(drawPixel(y+i, x, colour) != 0){
            panic("ERROR could not draw pixel")
            return 1;
        }
    }
    return 0;
}

int drawSquare(uint16_t x, uint16_t y, uint32_t colour, uint16_t len){
    for(uint16_t i = 0; i < len; i++){
        if(drawLineH(x+i, y ,colour, len) != 0){
            panic("ERROR could not draw line")
            return 1;
        }
    }
    return 0;
}

int DrawCube(uint16_t x, uint16_t y, uint32_t colour, uint16_t len){
    for(int i = 0; i < (len*181) >> 8; i++){
        drawSquare(x-i, y-i, len, colour);
    }
}

int pp_test_square(uint8_t mode, uint16_t x, uint16_t y, uint16_t len, uint32_t color, uint32_t delay){ 
    if(lpv_mode_select(mode) != 0){
        return 1;
    }

    if(lpv_mode_info_t(mode, &screen_res) != 0){
        return 1;
    }

    bytes_per_pixel = screen_res.bpp >> 3;
    vram_size = screen_res.x_res * screen_res.y_res * bytes_per_pixel;
    vram = video_map_phys(screen_res.phys_addr, vram_size);
    bzero(vram, vram_size);

    if(mode == Text_Mode){return 0};
    if(mode == Indexed){
       if( drawSquare(x, y, color, len) != 0){
        panic("ERROR could not draw square")
        return 1;
       }
       sleep(delay);
       if(lpv_set_mode(Text_Mode, 0) != 0){return 1;}
       return 0;
    }
    if (mode == Direct_Colour){
        uint32_t new_colour = 0;
        uint32_t red = (color >> 16) &0xFF;
        uint32_t green = (color >> 8) & 0xFF;
        uint32_t blue = color & 0xFF;
        red = red << (8 - screen_res.r_sz);
        green = green << (8 - screen_res.g_sz);
        blue = blue << (8 - screen_res.b_sz);
        new_colour |= (red << screen_res.r_pos);
        new_colour |= (green << screen_res.g_pos);
        new_colour |= blue;
        if(drawSquare(x, y, color, len) != 0){
        panic("ERROR could not draw square")
        return 1;
       }
       sleep(delay);
       f(lpv_set_mode(Text_Mode, 0) != 0){return 1;}
       return 0;
    }
}
