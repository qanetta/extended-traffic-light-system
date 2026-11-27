#ifndef SOFTWARE_TIMER_H
#define SOFTWARE_TIMER_H

#include <stdint.h>
#include "global.h"

/* Software timer cho may trang thai.
 * Timer_Run() la 1 task 10 ms cua scheduler (KHONG chay trong ngat)
 * -> FSM va timer cung chay o vong while, khong co race condition. */
#define TIMER_COUNT     4
#define TIMER_TICK_MS   10

void     Timer_Set(uint8_t id, uint32_t duration_ms);   /* bat dau dem, xoa co het han */
void     Timer_Run(void);                               /* task 10 ms */
uint8_t  Timer_IsExpired(uint8_t id);                   /* 1 khi da het thoi gian (giu nguyen cho toi lan set sau) */
uint32_t Timer_GetRemainingMs(uint8_t id);              /* thoi gian con lai, dung de dem nguoc tren LCD */

#endif /* SOFTWARE_TIMER_H */
