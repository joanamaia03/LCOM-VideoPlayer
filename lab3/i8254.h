#ifndef _LCOM_I8254_H_
#define _LCOM_I8254_H_

#include <lcom/lcf.h>

#define TIMER_FREQ 1193181
#define TIMER0_IRQ 0

/*Port addresses*/
#define TIMER_CTRL 0x43
#define TIMER_0 0x40
#define TIMER_1 0x41
#define TIMER_2 0x42
#define SPEAKER_CTRL 0x61

/*Control Word*/
#define TIMER0_CSEL 0x00
#define TIMER1_CSEL BIT(6)
#define TIMER2_CSEL BIT(7)
#define LSB_CINIT BIT(4)
#define MSB_CINIT BIT(5)
#define LSB_MSB (BIT(4)|BIT(5))
#define TIMER0_CMODE 0x00
#define TIMER1_CMODE BIT(1)
#define TIMER2_CMODE BIT(2)
#define TIMER3_CMODE (BIT(1)|BIT(2))
#define TIMER4_CMODE BIT(3)
#define TIMER5_CMODE (BIT(1)|BIT(3))
#define BCD_BIN 0x00
#define BCD 0x01

/*Read-Back Command*/
#define RB_COMMAND (BIT(6)|BIT(7))
#define RB_CMD_COUNT BIT(5)
#define RB_CMD_SATUS BIT(4)
#define RB_CMD_TIMER(n) BIT((n)+1)

#endif /* _LCOM_I8254_H */