#include "keyboard.h"

static int kbd_hook_id = 1;
static uint8_t scancode;

int(kbc_subscribe_int)(uint8_t *bit_no){
    *bit_no = kbd_hook_id;
    if(sys_irqsetpolicy(KBC_IRQ, IRQ_REENABLE|IRQ_EXCLUSIVE, &kbd_hook_id )!=OK){
        printf("Erro");
        return 1;
    }
    return 0;
}
int(kbc_unsubscribe_int)(){
    if(sys_irqrmpolicy(&kbd_hook_id)!=OK){
        printf("Erro");
        return 1;
    }
    return 0;
}

void(kbc_ih)(){
    uint8_t status;
    uint8_t aux_return=0;
    uint8_t tent=0;
    uint8_t dados;
    while(tent<5){
        /*verificar se converte para 8 bits*/
        if(util_sys_inb(STAT_REG,&status)!=0){
            scancode=0x00;
            printf("Erro");
            aux_return =1;
            return;
        }
        /*verificar se há dados invalidos*/
        if((status & (PARITY|TIMEOUT))!=0){
            scancode=0x00;
            aux_return=1;
            return;
        }
        /*verificar se há dados para ler*/
        if(status & OBF){
            if(util_sys_inb(OUT_BUF,&dados)!=0){
                scancode=0x00;
                printf("Erro");
                aux_return=1;
                return;
            }
            aux_return=dados;

            /*ler o valor se der erro mas não faz nada*/
            if(aux_return==1){
                continue;
            }
            util_sys_inb(OUT_BUF,&scancode);
            return;
        }
        tickdelay(micros_to_ticks(DELAY_US));
        tent++;
    }
    printf("Erro após 5 tentativas");
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

uint8_t scan;
uint32_t count_kbc;
int(kbd_scan_esc)() {
    int ipc_status;
    message msg;
    while( scancode!=BREAKCODE_ESC) {
        if(driver_receive(ANY, &msg, &ipc_status)!=0){
        printf("Erro");
        continue;
        }
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                if(msg.m_notify.interrupts){
                    kbc_ih();
                    scan=scancode;
                }
                break;
                    default:
                break;  
            }
        }
    }
    if(kbd_print_no_sysinb(count_kbc)!=0){return 1;}  
    return 0;
}
int(kbd_scan_space)() {
    int ipc_status;
    message msg;
    while( scancode!=BREAKCODE_SPACE) {
        if(driver_receive(ANY, &msg, &ipc_status)!=0){
        printf("Erro");
        continue;
        }
        if(is_ipc_notify(ipc_status)){
            switch(_ENDPOINT_P(msg.m_source)){
                case HARDWARE:
                if(msg.m_notify.interrupts){
                    kbc_ih();
                    scan=scancode;
                }
                break;
                    default:
                break;  
            }
        }
    }
    if(kbd_print_no_sysinb(count_kbc)!=0){return 1;}  
    return 0;
}
