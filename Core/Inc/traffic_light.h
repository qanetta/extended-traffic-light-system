#ifndef TRAFFIC_LIGHT_H
#define TRAFFIC_LIGHT_H

#include <stdint.h>
#include "global.h"

/* 2 module den giao thong, moi module 2 chan A, B (User Label trong .ioc):
 *   Tuyen 1: A = D3 (PB3, TL1_A), B = D4 (PB5, TL1_B)
 *   Tuyen 2: A = D5 (PB4, TL2_A), B = D6 (PB10, TL2_B)
 *   (da kiem tra tren mach: chan so nho cua khe GROVE la A)
 * Ma AB: 00 tat, 01 vang, 10 xanh, 11 do -> gia tri enum = ma AB
 * Neu thay vang/xanh bi doi cho nhau: A va B bi nguoc, doi label trong .ioc.
 */
typedef enum {
	TL_OFF   = 0,   /* A=0 B=0 */
	TL_AMBER = 1,   /* A=0 B=1 */
	TL_GREEN = 2,   /* A=1 B=0 */
	TL_RED   = 3    /* A=1 B=1 */
} TrafficColor;

#define LANE_1      0
#define LANE_2      1
#define LANE_COUNT  2

/* Khoa an toan trong traffic_set / Traffic_SetBoth: neu yeu cau lam ca 2 tuyen
 * cung duoc di (xanh hoac vang) thi module chuyen ca 2 tuyen sang DO va tra ve 0.
 * Hop le tra ve 1. */
void         Traffic_Init(void);                                       /* 2 tuyen tat */
uint8_t      Traffic_Set(uint8_t lane, TrafficColor color);            /* doi 1 tuyen, tuyen kia giu nguyen */
uint8_t      Traffic_SetBoth(TrafficColor lane1, TrafficColor lane2); /* doi 2 tuyen cung luc */
TrafficColor Traffic_Get(uint8_t lane);

#endif /* TRAFFIC_LIGHT_H */
