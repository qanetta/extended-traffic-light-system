#include "timing.h"

static uint8_t timing_green = 3;
static uint8_t timing_amber = 2;
static uint8_t timing_red   = 5;    /* luon = timing_green + timing_amber */

uint8_t Timing_GetRed(void)   { return timing_red;   }
uint8_t Timing_GetGreen(void) { return timing_green; }
uint8_t Timing_GetAmber(void) { return timing_amber; }

uint8_t Timing_IsValid(uint8_t green, uint8_t amber){
	return green >= TIME_MIN && amber >= TIME_MIN && green + amber <= TIME_RED_MAX;
}

uint8_t Timing_Set(uint8_t green, uint8_t amber){
	if (!Timing_IsValid(green, amber)) return 0;
	timing_green = green;
	timing_amber = amber;
	timing_red   = green + amber;
	return 1;
}
