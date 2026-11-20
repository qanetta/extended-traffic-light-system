#include "task_led_blinking.h"

void ledBlinking(){
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
