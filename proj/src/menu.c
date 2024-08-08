#include "graphic.h"
#include "menu.h"
#include <lcom/xpm.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"
#include "VideoCodec.h"

#include "img/background_menu.xpm"
#include "img/mediaplayerblack.xpm"
#include "img/pause.XPM"
#include "img/play-buttton.XPM"
#include "img/pimba.xpm"
#include "img/char.XPM"

static int minute = 0;
static uint8_t ihkeyboard;
//videores=640x360
void(ih_enable)(){
    kbc_subscribe_int(&ihkeyboard);
    if(graphic_set_VBE_mode(0x115)!=0){
        return;
    }
    init_draw();
}


void(init_draw)(){
    
    xpm_image_t img;
    xmp_draw(img,background_menu,0,0);
    xpm_image_t img2;
    xmp_draw(img2,mediplayerblack,100,40);
    char teste[] = "PRESS SPACE TO PLAY THE FILE!";
    xpm_image_t img3;
    int i = 0;
    while(teste[i]!='\0'){
        xmp_draw_char(img3,character,(153)+(i*17),200,teste[i]);
        i++;
    }
    //adicionar rtc
    char file[] = "FIREFLIES.MP4";
    xpm_image_t img4;
    int j = 0;
    while(file[j]!='\0'){
        xmp_draw_char(img4,character,(325)+(j*17),300,file[j]);
        j++;
    }
    char min[5];
    sprintf(min,"%d",minute);
    char add1[] = "ADDED AT ";
    char add2[] = " MINUTES AGO!";
    strcat(add1,min);
    strcat(add1,add2);
    xpm_image_t img5;
    int k = 0;
    while(add1[k]!='\0'){
        xmp_draw_char(img5,character,(189)+(k*17),400,add1[k]);
        k++;
    }
    while(kbd_scan_space()!=0){
        continue;

    }
    draw_video_menu();
    
   
}
void(draw_video_menu)(){
    //int w;
    //int h;
    //char* b = 0x00;
    //uint16_t x = 80;
    //uint16_t y = 0;
    if(graphic_set_VBE_mode(0x115)!=0){
        return;
    }
    //readFrame(&w, &h, b);
    //draw_frame(x, y, w, h, b);
    //graphic_draw_rectangle(1,600,700,150,GREY_COLOR);
    graphic_draw_rectangle(0,360,800,240,GREY_COLOR);
    graphic_draw_rectangle(0,0,80,360,GREY_COLOR);
    graphic_draw_rectangle(720,0,80,360,GREY_COLOR);
    //graphic_draw_rectangle(80,0,640,360,0);
    xpm_image_t img;
    xmp_draw(img,pausebutton,378,526);
    char teste[] = "PRESS ESC TO EXIT!";
    for(int i = 0;i<18;i++){
        xmp_draw_char(img,character,(10)+(i*17),400,teste[i]);
    }
    videoCodec();
    kbc_unsubscribe_int();
    vg_exit();
}

