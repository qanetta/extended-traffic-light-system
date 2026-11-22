#include "scheduler.h"

/* Mang task da sap xep. Chon Array thay vi Linked List vi:
 *  - SCH_MAX_TASKS nho (10): dich mang khi chen/xoa chi vai chuc lenh
 *  - khong can malloc (tranh phan manh heap tren MCU 20 KB RAM)
 *  - du lieu lien tuc, de xem trong debugger */
static sTask    SCH_tasks_G[SCH_MAX_TASKS];
static uint8_t  SCH_count;            /* so task dang co trong mang */
static uint32_t SCH_next_id;          /* ID cap cho task ke tiep (bo qua 0) */

/* Dung chung voi ngat TIM2 -> volatile */
static volatile uint32_t SCH_lost_ticks;   /* so tick troi qua khi task dau da den han ma chua duoc chay */

/* ---------------- Vung gang (critical section) ----------------
 * SCH_Update chay trong ngat va sua SCH_tasks_G[0]. Neu vong while dang dich mang
 * ma ngat chen vao giua thi du lieu hong -> tat ngat trong luc sua mang.
 * Luu PRIMASK de goi long nhau van dung. */
static uint32_t sch_lock(void){
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	return primask;
}

static void sch_unlock(uint32_t primask){
	__set_PRIMASK(primask);
}

/* Task dau danh sach co Delay = 0 la da den han */
static void sch_mark_head(void){
	if (SCH_count > 0 && SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;
}

/* Chen task vao dung vi tri theo thoi diem chay. Goi khi da khoa ngat. */
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

/* Xoa task o vi tri i. Goi khi da khoa ngat. */
static void sch_remove_at(uint8_t i){
	/* Thoi gian cua task bi xoa cong don sang task ke tiep de cac task phia sau giu dung thoi diem */
	if (i + 1 < SCH_count) SCH_tasks_G[i + 1].Delay += SCH_tasks_G[i].Delay;

	for (uint8_t k = i; k + 1 < SCH_count; k++) SCH_tasks_G[k] = SCH_tasks_G[k + 1];
	SCH_count--;

	SCH_tasks_G[SCH_count].pTask  = 0;
	SCH_tasks_G[SCH_count].TaskID = NO_TASK_ID;
	sch_mark_head();
}

static uint32_t sch_new_id(void){
	if (++SCH_next_id == NO_TASK_ID) SCH_next_id = 1;   /* tran 32 bit thi bo qua 0 */
	return SCH_next_id;
}

/* ---------------- Public ---------------- */

void SCH_Init(void){
	uint32_t pm = sch_lock();
	for (uint8_t i = 0; i < SCH_MAX_TASKS; i++){
		SCH_tasks_G[i].pTask  = 0;
		SCH_tasks_G[i].Delay  = 0;
		SCH_tasks_G[i].Period = 0;
		SCH_tasks_G[i].RunMe  = 0;
		SCH_tasks_G[i].TaskID = NO_TASK_ID;
	}
	SCH_count      = 0;
	SCH_next_id    = 0;
	SCH_lost_ticks = 0;
	sch_unlock(pm);
}

uint32_t SCH_Add_Task(void (*pFunc)(void), uint32_t DELAY, uint32_t PERIOD){
	if (pFunc == 0) return NO_TASK_ID;
	uint32_t pm = sch_lock();
	uint32_t id = (SCH_count < SCH_MAX_TASKS) ? sch_insert(pFunc, DELAY, PERIOD, sch_new_id()) : NO_TASK_ID;
	sch_unlock(pm);
	return id;
}

uint8_t SCH_Delete_Task(uint32_t taskID){
	if (taskID == NO_TASK_ID) return 0;
	uint32_t pm = sch_lock();
	for (uint8_t i = 0; i < SCH_count; i++){
		if (SCH_tasks_G[i].TaskID == taskID){
			sch_remove_at(i);
			sch_unlock(pm);
			return 1;
		}
	}
	sch_unlock(pm);
	return 0;
}

/* O(1): chi xet task dau danh sach */
void SCH_Update(void){
	if (SCH_count == 0) return;

	/* Task dau da den han nhung vong while chua kip chay (vi task truoc chay qua 10 ms):
	 * khong bo mat tick nay, ghi lai de dispatcher bu cho cac task phia sau */
	if (SCH_tasks_G[0].RunMe){
		SCH_lost_ticks++;
		return;
	}
	if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
	if (SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;
}

void SCH_Dispatch_Tasks(void){
	for (;;){
		uint32_t pm = sch_lock();

		/* Bu cac tick bi tre (neu co) cho task dau moi */
		if (SCH_count > 0 && !SCH_tasks_G[0].RunMe && SCH_lost_ticks > 0){
			SCH_lost_ticks--;
			if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;
			sch_mark_head();
			sch_unlock(pm);
			continue;
		}

		if (SCH_count == 0 || !SCH_tasks_G[0].RunMe){
			if (SCH_count == 0) SCH_lost_ticks = 0;
			sch_unlock(pm);
			return;                       /* khong con task den han */
		}

		/* Lay task dau ra khoi mang. Task ke tiep co Delay = 0 (cung han) se duoc danh dau RunMe */
		sTask t = SCH_tasks_G[0];
		sch_remove_at(0);
		sch_unlock(pm);

		t.pTask();                        /* chay ngoai vung khoa: ngat TIM2 van hoat dong */

		/* Task tuan hoan: them lai, giu nguyen TaskID.
		 * One-shot (Period = 0): khong them lai -> tu xoa khoi danh sach */
		if (t.Period > 0){
			pm = sch_lock();
			sch_insert(t.pTask, t.Period, t.Period, t.TaskID);
			sch_unlock(pm);
		}
	}
}
