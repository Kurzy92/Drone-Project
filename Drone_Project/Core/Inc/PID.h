/*
 * PID.h
 *
 *  Created on: Apr 13, 2024
 *      Author: eyalk
 */

#ifndef INC_PID_H_
#define INC_PID_H_

#include "stm32f7xx_hal.h"

typedef struct {

	/* PID Gains */
	float kp; //proportional gain
	float ki; //integral gain
	float kd; //derivative gain

	float tau;

	/* Period time(in seconds) */
	float period;

	/* PID  */
	float error;
	float proportional_element;

	/* PID "memory" */
	float derivativation_element;
	float integrator_element;
	float prev_error;
	float prev_measurement;

	/* PID output */
	float output;

	/* Integrator limits */
	float min_integrator;
	float max_integrator;

	/* PID Output limits */
	float min_output_val;
	float max_output_val;

}pid_t;

void PID_Init(pid_t* pid);
void PID_setKp(pid_t* pid, float Kp);
void PID_setKd(pid_t* pid, float Ki);
void PID_setKi(pid_t* pid, float Kd);
float PID_getOutput(pid_t* pid, float current_measurement, float desired_measurement);
#endif /* INC_PID_H_ */
