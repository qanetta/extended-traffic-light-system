#include "scheduler.h"

/* Mang task da sap xep. Chon Array thay vi Linked List vi:
 *  - SCH_MAX_TASKS nho (10): dich mang khi chen/xoa chi vai chuc lenh
 *  - khong can malloc (tranh phan manh heap tren MCU 20 KB RAM)
 *  - du lieu lien tuc, de xem trong debugger */
static sTask    SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t  SCH_count;            /* so task dang co trong mang */
static uint32_t SCH_next_id;          /* ID cap cho task ke tiep (bo qua 0) */

/* Task dau danh sach co Delay = 0 la da den han */
static void sch_mark_head(void){
	if (SCH_count > 0 && SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;
}

/* Chen task vao dung vi tri theo thoi diem chay. */
static uint32_t sch_insert(void (*pFunc)(void), uint32_t delay, uint32_t period, uint32_t id){
	if (SCH_count >= SCH_MAX_TASKS) return NO_TASK_ID;

	/* Di qua cac task chay truoc (hoac cung luc) task moi, tru dan Delay.
	 * Dung >= de task moi dung SAU cac task cung han -> chay theo thu tu them (FIFO). */
	uint8_t i = 0;
	while (i < SCH_count && delay >= SCH_tasks_G[i].Delay){
		delay -= SCH_tasks_G[i].Delay;
		i++;
	}

	/* Task dung ngay sau vi tri chen: Delay cua no gio tinh tu task moi */
	if (i < SCH_count) SCH_tasks_G[i].Delay -= delay;

	for (uint8_t k = SCH_count; k > i; k--) SCH_tasks_G[k] = SCH_tasks_G[k - 1];

	SCH_tasks_G[i].pTask  = pFunc;
	SCH_tasks_G[i].Delay  = delay;
	SCH_tasks_G[i].Period = period;
	SCH_tasks_G[i].RunMe  = 0;
	SCH_tasks_G[i].TaskID = id;
	SCH_count++;

	sch_mark_head();
	return id;
}

static uint32_t sch_new_id(void){
	if (++SCH_next_id == NO_TASK_ID) SCH_next_id = 1;   /* tran 32 bit thi bo qua 0 */
	return SCH_next_id;
}

/* ---------------- Public ---------------- */

void SCH_Init(void){
	for (uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		SCH_tasks_G[i].pTask  = 0;
		SCH_tasks_G[i].Delay  = 0;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].RunMe  = 0;
		SCH_tasks_G[i].TaskID = NO_TASK_ID;
	}
	SCH_count      = 0;
	SCH_next_id    = 0;
}

uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD){
	if (pFunc == 0) return NO_TASK_ID;
	return (SCH_count < SCH_MAX_TASKS) ? sch_insert(pFunc, DELAY, PERIOD, sch_new_id()) : NO_TASK_ID;
}
