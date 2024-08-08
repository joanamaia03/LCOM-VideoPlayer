#include <lcom/lcf.h>

#include <stdint.h>

int (util_get_LSB)(uint16_t val, uint8_t *lsb){
    if(lsb==NULL){return 1;}
    *lsb=(uint8_t)val;
    return 0;
}
int (util_get_MSB)(uint16_t val, uint8_t *msb){
    if(msb==NULL){return 1;}
    *msb=(uint8_t)val>>8;
    return 0;
}
int (util_sys_inb)(int port, uint8_t *byte){
    if(byte==NULL){return 1;}
    return (sys_inb(port,(uint32_t*)byte));
}
