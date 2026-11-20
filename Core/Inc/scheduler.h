#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include "global.h"   /* CMSIS: __disable_irq, __get_PRIMASK */

/* Scheduler cooperative, SCH_Update O(1)
 *
 * Danh sach task la mang sap xep theo thoi diem chay. Moi task luu Delay TUONG DOI
 * so voi task dung truoc no, vi du 3 task chay sau 2, 5, 5 tick duoc luu la [2, 3, 0].
 * -> SCH_Update (trong ngat) chi can giam Delay cua task dau tien: O(1).
 * -> Cong viec nang (tim vi tri chen, dich mang) don sang SCH_Add_Task: O(n), chay o vong while.
 *
 * Don vi Delay/Period la TICK (1 tick = SCH_TICK_MS = chu ky ngat TIM2).
 */

#define SCH_TICK_MS     10
#define SCH_MS(ms)      ((ms) / SCH_TICK_MS)   /* doi ms -> tick */
#define SCH_MAX_TASKS   10
#define NO_TASK_ID      0

typedef struct {
	void   (*pTask)(void);   /* con tro toi ham task */
	uint32_t Delay;          /* so tick con lai, tuong doi so voi task dung truoc */
	uint32_t Period;         /* chu ky (tick), 0 = one-shot task */
	uint8_t  RunMe;          /* co bao task dau danh sach da den han */
	uint32_t TaskID;         /* ID duy nhat, dung de xoa task */
} sTask;

void     SCH_Init(void);
uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD);   /* tra ve TaskID, NO_TASK_ID neu day */
uint8_t  SCH_Delete_Task(uint32_t taskID);                                      /* 1 = da xoa, 0 = khong tim thay */
void     SCH_Update(void);           /* goi trong ngat timer */
void     SCH_Dispatch_Tasks(void);   /* goi trong while(1) */
#endif /* SCHEDULER_H */
