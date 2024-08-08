///Provides functions that keep and give information about the time that a filed was added
///
///The function rtc_wait_valid(void) waits until the RTC is ready to be accessed.
///
///The function rtc_enable() activates the RTC.
///
///The function rtc_disable() disables the RTC.
///
///The function rtc_insert_instruction(uint8_t arrpos, uint8_t *ans) reads the value from a specific RTC register.

#ifndef _RTC_H
#define _RTC_H
#include <lcom/lcf.h>
#include <lcom/utils.h>

#define RTC_UIP BIT(7)
#define RTC_ADDR_REG 0x70
#define RTC_DATA_REG 0X71
#define RTC_REG_A 10
#define RTC_REG_B 11
#define RTC_REG_C 12
#define RTC_REG_D 13

void (rtc_wait_valid)(void);

void (rtc_enable)(void);

void (rtc_disable)(void);

int(rtc_insert_instruction)(uint8_t arrpos,uint8_t *ans);

#endif
