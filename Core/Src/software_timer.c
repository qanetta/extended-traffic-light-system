#include "software_timer.h"

static uint32_t timer_counter[TIMER_COUNT];   /* so tick con lai */
static uint8_t  timer_flag[TIMER_COUNT];      /* 1 = het han */

void Timer_Set(uint8_t id, uint32_t duration_ms){
	if (id >= TIMER_COUNT) return;
	timer_counter[id] = (duration_ms + TIMER_TICK_MS - 1) / TIMER_TICK_MS;   /* lam tron len */
	timer_flag[id]    = (timer_counter[id] == 0);
}

void Timer_Run(void){
	for (uint8_t i = 0; i < TIMER_COUNT; i++){
		if (timer_counter[i] > 0){
			timer_counter[i]--;
			if (timer_counter[i] == 0) timer_flag[i] = 1;
		}
	}
}

uint8_t Timer_IsExpired(uint8_t id){
	return (id < TIMER_COUNT) ? timer_flag[id] : 1;
}

uint32_t Timer_GetRemainingMs(uint8_t id){
	return (id < TIMER_COUNT) ? timer_counter[id] * TIMER_TICK_MS : 0;
}
