#include <lcom/lcf.h>

#include <lcom/lab3.h>

#include <stdbool.h>
#include <stdint.h>

#include "i8042.h"
#include "keyboard.h"

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}
uint8_t scancode;
uint32_t count_kbc;
unsigned int counter;
int(kbd_test_scan)() {
int hook_id = 0;
(kbc_subscribe_int)(&hook_id);
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
            kbd_print_scancode(!(scancode & MAKECODE_ESC),scancode==TWO_BYTE_SCAN?2:1,&scancode);
          }
          break;
        default:
          break;  
      }
    }
  }
  if(kbc_unsubscribe_int(hook_id)!=0){return 1;}
  if(kbd_print_no_sysinb(count_kbc)!=0){return 1;}  
  return 0;
}

int(kbd_test_poll)() {
  bool flag=false;
  uint8_t bytes[2];
  while(scancode!=BREAKCODE_ESC){
    kbc_ih();
    if(scancode==0x00){
      tickdelay(micros_to_ticks(DELAY_US));
      continue;
    }
    if(flag){
      flag=true;
      bytes[1]=scancode;
      kbd_print_scancode(!(scancode & BIT(7)),2,bytes);
    }
    else{
      bytes[0]=scancode;
      if(scancode==TWO_BYTE_SCAN){
        flag=true;
      }
      else{
        kbd_print_scancode(!(scancode & BIT(7)),1,bytes);
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }
  if(kbd_enable()!=0){
      return 2;
  }
  if(kbd_print_no_sysinb(count_kbc)!=0){return 1;}
  return 0;
}

/*int(kbd_test_timed_scan)(uint8_t n) {
    uint8_t time=n;
    int ipc_status;
    message msg;
    uint32_t irq_set_timer = BIT(0); 
    uint32_t irq_set_kbd = BIT(1);
    uint8_t bit_no;
    uint8_t bytes[2];
    bool flag = false;
    if (kbc_subscribe_int(&bit_no) != 0) {return 1;}
    if (timer_subscribe_int(&bit_no) != 0) {return 1;}
    while(time!=0 && scancode!= BREAKCODE_ESC){
      if(driver_receive(ANY, &msg, &ipc_status)!=0){
        printf("Erro");
        continue;
      }
      if(is_ipc_notify(ipc_status)){
        switch (_ENDPOINT_P(msg.m_source)){
          case HARDWARE:
            if(msg.m_notify.interrupts & irq_set_kbd){
              kbc_ih();
              if(flag){
                flag=false;
                bytes[1]=scancode;
                kbd_print_scancode(!(scancode & BIT(7)),2,bytes);
              }
              else{
                bytes[0]=scancode;
                if(scancode==TWO_BYTE_SCAN){
                  flag=true;
                }
                else{
                  kbd_print_scancode(!(scancode & BIT(7)),1,bytes);
                }
                time=n;
                counter=0;
                continue;
              }
              if(msg.m_notify.interrupts & irq_set_timer){
                timer_int_handler();
                if(counter%60==0 && counter!=0){
                  time--;
                }
                break;
              }
            }
            default:
              break;
        }
      }
    }
    if(timer_unsubscribe_int()!=0){return 1;}
    if(kbc_unsubscribe_int(hook_id)!=0){return 1;}
    kbd_print_no_sysinb(count_kbc);
    return 0;
}*/

