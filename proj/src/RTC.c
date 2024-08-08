#include "RTC.h"

void (rtc_wait_valid)(void){
    uint8_t regA = 0;
    do{
        rtc_disable();
        sys_outb(RTC_ADDR_REG,RTC_REG_A);
        util_sys_inb(RTC_DATA_REG,&regA);
        rtc_enable();

    } while(regA & RTC_UIP);

}

void (rtc_enable)(){
    uint8_t regB = 0;
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    util_sys_inb(RTC_DATA_REG,&regB);
    regB = regB | 0x7;
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB); 
}

void(rtc_disable)(){
    uint8_t regB = 0;
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    util_sys_inb(RTC_DATA_REG,&regB);
    regB = regB & 0x8F;
    sys_outb(RTC_ADDR_REG,RTC_REG_B);
    sys_outb(RTC_DATA_REG,regB);
}

int(rtc_insert_instruction)(uint8_t arrpos,uint8_t *ans){
    sys_outb(RTC_ADDR_REG,arrpos);
    util_sys_inb(RTC_DATA_REG,ans);
    return 0;
}

