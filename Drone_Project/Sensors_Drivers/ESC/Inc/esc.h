/*
 * esc.h
 *
 *  Created on: May 5, 2024
 *      Author: eyalk
 */

#ifndef ESC_INC_ESC_H_
#define ESC_INC_ESC_H_

#include "stm32f7xx_hal.h"
#include "arm_neon.h"

enum motor_location {
	front_right = 1,
	front_left,
	rear_right,
	rear_left
};

typedef struct {
	enum motor_location motor_loc;
	float32_t PWM_signal;

} ESC_t;

void ESC_Init();

#endif /* ESC_INC_ESC_H_ */
