#include <lcom/lcf.h>
#include "i8042.h"
#include "utilsL3.h"

int(kbc_subscribe_int)(int *bit_no){
    int hook_id = 0;
    if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &hook_id )!=OK){
        printf("Erro");
        return 1;
    }
    *bit_no = hook_id;
    return 0;
}
int(kbc_unsubscribe_int)(int hook_id){
    if(sys_irqrmpolicy(&hook_id)!=OK){
        printf("Erro");
        return 1;
    }
    return 0;
}
void(kbc_ih)(){
    uint8_t status;
    uint8_t aux_return=0;
    uint8_t dados;
    uint8_t scancode;
    /*verificar se converte para 8 bits*/
    if(util_sys_inb(STAT_REG,&status)!=0){
        scancode=0x00;
        printf("Erro");
        aux_return =1;
    }
    /*verificar se há dados invalidos*/
    if((status & (PARITY|TIMEOUT))!=0){
        scancode=0x00;
        aux_return=1;
    }
    /*verificar se há dados para ler*/
    if(status & OBF){
        if(util_sys_inb(OUT_BUF,&dados)!=0){
            scancode=0x00;
            printf("Erro");
            aux_return=1;
        }
        aux_return=dados;
    }
    tickdelay(micros_to_ticks(DELAY_US));
    aux_return=1;
}


int(kbd_enable)(){
    uint8_t cmd_byte;
    if(sys_outb(COMMAND_KBC, READ_COMMAND)!=0){
        return 4;
    }
    if(util_sys_inb(OUT_BUF, &cmd_byte)!=0){
        return 4;
    }
    cmd_byte=(cmd_byte | BIT(0));
    if(sys_outb(COMMAND_KBC,OUT_BUF)!=0){
        return 4;
    }
    if(sys_outb(WRITE_COMMAND, cmd_byte)!=0){
        return 4;
    }
    return 0;
}


