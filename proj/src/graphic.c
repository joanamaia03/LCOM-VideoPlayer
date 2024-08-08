#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "graphic.h"
vbe_mode_info_t mode_info;
static uint16_t hres; /* XResolution */
static uint16_t vres; /* YResolution */
static uint8_t *video_mem;
static int vram_base;
static int vram_size;



int(graphic_set_VBE_mode)(uint16_t mode){
    memset(&mode_info,0,sizeof(mode_info));
    if(vbe_get_mode_info(mode,&mode_info)!=0){return -1;}
    int r;
    struct minix_mem_range mr; /* physical memory range */
    vram_base = mode_info.PhysBasePtr; /* VRAM’s physical addresss */
    vram_size = mode_info.XResolution*mode_info.YResolution * (mode_info.BitsPerPixel + 7)/8;
    hres = mode_info.XResolution;
    vres = mode_info.YResolution; /* VRAM’s size, but you can use
    the frame-buffer size, instead */
    /* Allow memory mapping */
    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;
    if(0 != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
    panic("sys_privctl (ADD_MEM) failed: %d\n", r);
    /* Map memory */
    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);
    if(video_mem == MAP_FAILED)
    panic("couldn’t map video memory");
    reg86_t reg;
    memset(&reg,0,sizeof(reg));
    reg.ax = 0x4F02; // VBE call, function 02 -- set VBE mode
    reg.bx = 1<<14|mode; // set bit 14: linear framebuffer
    reg.intno = 0x10;
    if( sys_int86(&reg) !=0 ) {
    printf("set_vbe_mode: sys_int86() failed \n");
    return 1;
    }
    return 0;
}
int(changePixelColor)(uint16_t x,uint16_t y,uint32_t color){
    if(color == xpm_transparency_color(XPM_8_8_8)) return 0;
    unsigned bpp = (mode_info.BitsPerPixel+7)/8;
    uint8_t *base = (uint8_t *) video_mem + (y * hres + x) * bpp;
    for (uint8_t j = 0; j < bpp; ++j) {
		  *base = color >> (j * 8);
		  base++;
	}
  return 0;
}

int(xmp_draw)(xpm_image_t img,xpm_map_t xpm, uint16_t x, uint16_t y){
  uint8_t *mapa = xpm_load(xpm,XPM_8_8_8,&img);
  uint32_t color;
  for(int i = 0;i<img.height;i++){
    for(int j = 0;j<img.width;j++){
      color = *mapa;
      mapa++;
      color |= (*mapa << 8);
      mapa++;
      color |= (*mapa << 16);
      changePixelColor(x+j,i+y,color);
      mapa++;
    }
  }
  return 0;
}
int(draw_frame)(uint16_t x, uint16_t y, int width, int height, unsigned char* buffer){
  uint32_t color;
  for(int i = 0;i<height;i++){
    for(int j = 0;j<width;j++){
      color = *buffer;
      buffer++;
      color |= (*buffer << 8);
      buffer++;
      color |= (*buffer << 16);
      changePixelColor(x+j,i+y,color);
      buffer++;
    }
  }
  return 0;
}

int(xmp_draw_char)(xpm_image_t img,xpm_map_t xpm, uint16_t x, uint16_t y,char a){
  uint8_t *mapa = xpm_load(xpm,XPM_8_8_8,&img);
  uint32_t color;
  uint32_t posx;
  uint32_t posy;
  char matrix[8][8] = {
  {' ','!','"','#','$','%','&',146},
  {'(',')','*','+',',','-','.','/'},
  {'0','1','2','3','4','5','6','7'},
  {'8','9',':',';','<','=','>','?'},
  {'@','A','B','C','D','E','F','G'},
  {'H','I','J','K','L','M','N','O'},
  {'P','Q','R','S','T','U','V','W'},
  {'X','Y','Z','[','\\',']','^','_'}};
  for(int f = 0;f<8;f++){
    for(int g = 0;g<8;g++){
      if(matrix[f][g]==a){
        posy=f*32;
        posx=g*32;
        mapa+=((f*256)+g)*96;
      }
    }
  }
  for(int i = 0;i<32;i++){
    for(int j = 0;j<14;j++){
      color = *mapa;
      mapa++;
      color |= (*mapa << 8);
      mapa++;
      color |= (*mapa << 16);
      changePixelColor(x+j,i+y,color);
      mapa++;
    }
    mapa+=(242*3);
  }
  return 0;

}

int(vg_draw_hline)(uint16_t x,uint16_t y,uint16_t len,uint32_t color){
    for(unsigned j = 0;j<len;j++){
        if(changePixelColor(x+j,y,color)!= 0){return -1;}
    }
    return 0;
}

int(graphic_draw_rectangle)(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint32_t color){
    for(unsigned int i=0;i<height;i++){
        if(vg_draw_hline(x,y+i,width,color)!=0){return 1;}
    }
    return 0;
}
uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
  return (R(first) + j * step) % (1 << mode_info.RedMaskSize);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
  return (G(first) + i * step) % (1 << mode_info.GreenMaskSize);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
  return (B(first) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
}
uint32_t (R)(uint32_t first){
  return ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
}

uint32_t (G)(uint32_t first){
  return ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
}

uint32_t (B)(uint32_t first){
  return ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
}
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << mode_info.BitsPerPixel);
}

/* readFrame() used to read a pre stored frame file with required format. Just for debug purpose */
void (readFrame)(int* Pwidth, int* Pheight, char* buff2) { 
    //char* buff0 = 0x00;
    //char* buff1 = 0x00;
    //FILE* pFile;
    int pFile;
    //int result;
    //int w;
    //int h;
    //int maxChars =10;
    //char fname[20];
    //buff1 = (char*) malloc (sizeof(char)*10);

    //sprintf(fname, "./frame1.ppm");

    pFile = open ("./frame1.ppm"  , O_RDONLY );
    //pFile = fopen ("./frame1.ppm"  , "rb" );
    if (pFile==-1) {
        graphic_draw_rectangle(80,0,640,360,255);
        //panic("Could not open file %s\n","frame1.ppm" ); 
        //exit(1);
        return;
    }

    // Extract header and determine frame resolution 
    //fgets(buff1,maxChars,pFile);   // strip off first line
    //fgets(buff1,maxChars,pFile);   // read frame resolution
    //sscanf(buff1,"%d %d",&w ,&h);
    //fgets(buff1,maxChars,pFile);   // strip off third line
    //free(buff1);

    // allocate memory to contain the whole file:
    //buff0 = (char*) malloc (sizeof(char)*w*h*3);
    //if (buff0 == NULL){
        //panic("Error allocating Memory to store pixel data\n" );
        //exit (2);
    //}

    // Restore pixel data
    //result = fread (buff0,1,w*h*3,pFile);
    /*if (result != (w*h*3)){
        //panic("Error reading file %s. Incorrect number of bytes restored\n","frame1.ppm" );
        //exit(3);
    }*/
    /* the whole file is now loaded in the memory buffer. */

    // terminate
    //fclose (pFile);
    close (pFile);
    //free (buff0);
    //Pheight = &h;
    //Pwidth = &w;
    return;
}
