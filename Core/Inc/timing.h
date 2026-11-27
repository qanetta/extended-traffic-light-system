#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>
#include "global.h"

/* Bo thoi gian dang dung cho che do tu dong (don vi: giay).
 * Rang buoc luon dung: DO = XANH + VANG
 * -> tuyen 1 do dung bang luc tuyen 2 xanh + vang, 2 tuyen khop pha, khong bao gio cung xanh.
 * Chi doi duoc qua Timing_Set (co kiem tra hop le). */
#define TIME_MIN        1     /* giay */
#define TIME_RED_MAX    99    /* DO = XANH + VANG <= 99 de vua 2 chu so tren LCD */

uint8_t Timing_GetRed(void);
uint8_t Timing_GetGreen(void);
uint8_t Timing_GetAmber(void);
uint8_t Timing_IsValid(uint8_t green, uint8_t amber);   /* 1 = bo thoi gian hop le */
uint8_t Timing_Set(uint8_t green, uint8_t amber);       /* 1 = da luu, 0 = khong hop le, giu gia tri cu */

#endif /* TIMING_H */
