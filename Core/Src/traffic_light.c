#include "traffic_light.h"

typedef struct {
	GPIO_TypeDef *port_a;
	uint16_t      pin_a;
	GPIO_TypeDef *port_b;
	uint16_t      pin_b;
} LanePins;

/* Chan lay tu User Label trong .ioc (main.h) */
static const LanePins lanes[LANE_COUNT] = {
	{ TL1_A_GPIO_Port, TL1_A_Pin, TL1_B_GPIO_Port, TL1_B_Pin },
	{ TL2_A_GPIO_Port, TL2_A_Pin, TL2_B_GPIO_Port, TL2_B_Pin },
};

static TrafficColor current[LANE_COUNT];

/* Tuyen dang duoc phep di (xanh hoac vang) */
static uint8_t is_go(TrafficColor c){
	return c == TL_GREEN || c == TL_AMBER;
}

/* Ghi ca 4 chan. Moi port chi ghi 1 lan vao thanh ghi BSRR (RM0008 muc 9.2.5):
 * 16 bit thap = set, 16 bit cao = reset, ghi 1 lenh la doi dong thoi nhieu chan.
 * 4 chan den deu nam tren GPIOB -> 2 tuyen doi mau cung 1 thoi diem,
 * khong co trang thai trung gian (vd xanh -> vang ma thoang qua 00 = tat). */
static void write_outputs(void){
	struct { GPIO_TypeDef *port; uint32_t bsrr; } w[2 * LANE_COUNT];
	uint8_t n = 0;

	for (uint8_t l = 0; l < LANE_COUNT; l++){
		GPIO_TypeDef *port[2] = { lanes[l].port_a, lanes[l].port_b };
		uint16_t      pin[2]  = { lanes[l].pin_a,  lanes[l].pin_b  };
		uint8_t       level[2] = { (current[l] >> 1) & 1, current[l] & 1 };   /* A = bit 1, B = bit 0 */

		for (uint8_t k = 0; k < 2; k++){
			uint8_t j = 0;
			while (j < n && w[j].port != port[k]) j++;      /* gom theo port */
			if (j == n){ w[n].port = port[k]; w[n].bsrr = 0; n++; }
			w[j].bsrr |= level[k] ? pin[k] : ((uint32_t)pin[k] << 16);
		}
	}
	for (uint8_t j = 0; j < n; j++) w[j].port->BSRR = w[j].bsrr;
}

void Traffic_Init(void){
	current[LANE_1] = TL_OFF;
	current[LANE_2] = TL_OFF;
	write_outputs();
}

uint8_t Traffic_SetBoth(TrafficColor lane1, TrafficColor lane2){
	uint8_t ok = 1;
	if (is_go(lane1) && is_go(lane2)){
		/* Khong bao gio cho 2 tuyen cung di: chuyen ve trang thai an toan */
		lane1 = TL_RED;
		lane2 = TL_RED;
		ok = 0;
	}
	current[LANE_1] = lane1;
	current[LANE_2] = lane2;
	write_outputs();
	return ok;
}

uint8_t Traffic_Set(uint8_t lane, TrafficColor color){
	if (lane >= LANE_COUNT) return 0;
	return (lane == LANE_1) ? Traffic_SetBoth(color, current[LANE_2])
	                        : Traffic_SetBoth(current[LANE_1], color);
}

TrafficColor Traffic_Get(uint8_t lane){
	return (lane < LANE_COUNT) ? current[lane] : TL_OFF;
}
