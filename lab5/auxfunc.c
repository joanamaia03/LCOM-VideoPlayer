#include <lcom/lcf.h>
#include <machine/int86.h> // /usr/src/include/arch/i386

int vg_exit() {
    reg86_t r86;
    r86.intno = 0x10;
    r86.ah = 0x00;
    r86.al = 0x03;
    if(sys_int86(&reg86) != OK ) {
        printf("vg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}


#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) 			(((x) >> 4) & 0x0F000)
#define PB2OFF(x)			((x) & 0x0FFFF)
#define MODEINFO_SIZE		256

int vg_start(uint16_t mode) {
    reg86_t r86;
    memset(&r86, 0, sizeof(r86));
    r86.intno = 0x10;
    r86.ah = 0x4F;
    r86.al = 0x02;
    r86.bx = 0x0000;
    r86.bx |= mode;
    r86.bx |= (1<<14);

    if(sys_int86(&reg86) != OK ) {
        printf("vg_exit(): sys_int86() failed \n");
        return 1;
    }
    return 0;
}