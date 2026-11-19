#ifndef GLOBAL_H
#define GLOBAL_H

/* Khai bao dung chung toan du an.
 * Theo convention: chi include stm32f1xx.h, khong include header cua module khac.
 * Hien tai chua co bien/ham nao thuc su can pham vi toan cuc:
 *   - trang thai FSM   -> fsm_state.h
 *   - thoi gian den    -> timing.h
 *   - ban nhap tuning  -> fsm_tuning.h (getter) */
#include "stm32f1xx.h"

#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define TL2_B_Pin GPIO_PIN_10
#define TL2_B_GPIO_Port GPIOB
#define BTN3_Pin GPIO_PIN_7
#define BTN3_GPIO_Port GPIOC
#define LD1_Pin GPIO_PIN_9
#define LD1_GPIO_Port GPIOC
#define BTN1_Pin GPIO_PIN_8
#define BTN1_GPIO_Port GPIOA
#define BTN2_Pin GPIO_PIN_9
#define BTN2_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define TL1_A_Pin GPIO_PIN_3
#define TL1_A_GPIO_Port GPIOB
#define TL2_A_Pin GPIO_PIN_4
#define TL2_A_GPIO_Port GPIOB
#define TL1_B_Pin GPIO_PIN_5
#define TL1_B_GPIO_Port GPIOB
#define BTN4_Pin GPIO_PIN_6
#define BTN4_GPIO_Port GPIOB

#endif /* GLOBAL_H */
