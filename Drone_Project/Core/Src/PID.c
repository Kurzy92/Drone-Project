/**
 * @file PID.c
 * @brief Implementation of PID controller functions.
 *
 * This file provides the implementation of a PID (Proportional, Integral, Derivative) controller
 * including initialization and parameter setting functions. PID controllers are commonly used
 * in industrial control systems and other applications requiring continuously modulated control.
 *
 * @date Apr 13, 2024
 * @author eyalk
 */

#include "PID.h"

#define PERIOD_MS 1 ///< The period in milliseconds for the PID calculations.


/**
 * @brief Initializes a PID controller structure to zero.
 *
 * This function resets all PID parameters and state variables to zero, effectively
 * disabling the PID controller until its parameters are set.
 *
 * @param pid Pointer to the PID controller structure to initialize.
 */
void PID_Init(pid_t* pid) {
	pid->kd = 0.0;
	pid->ki = 0.0;
	pid->kp = 0.0;

	pid->error = 0.0;
	pid->prev_error = 0.0;
	pid->integrator_element = 0.0;
	pid->integrator_element = 0.0;
	pid->proportional_element = 0.0;
	pid->derivativation_element = 0.0;
	pid->output = 0.0;
}


/**
 * @brief Sets the proportional gain of the PID controller.
 *
 * @param pid Pointer to the PID controller structure.
 * @param Kp New proportional gain value.
 */
void PID_setKp(pid_t* pid, float Kp) {
	pid->kp = Kp;
}


/**
 * @brief Sets the integral gain of the PID controller.
 *
 * @param pid Pointer to the PID controller structure.
 * @param Ki New integral gain value.
 */
void PID_setKi(pid_t* pid, float Ki) {
	pid->ki = Ki;
}


/**
 * @brief Sets the derivative gain of the PID controller.
 *
 * @param pid Pointer to the PID controller structure.
 * @param Kd New derivative gain value.
 */
void PID_setKd(pid_t* pid, float Kd) {
	pid->kd = Kd;
}

/**
 * @brief Computes the output of the PID controller.
 *
 * This function calculates the PID control variable based on the current and desired states.
 * It updates the internal state of the PID controller for use in subsequent iterations.
 *
 * @param pid Pointer to the PID controller structure.
 * @param current_state The current state of the system being controlled.
 * @param desired_state The desired state of the system.
 * @return The control output from the PID controller.
 */
float PID_getOutput(pid_t* pid, float current_measurement, float desired_measurement) {
	/* Error Calculation */
	pid->error = desired_measurement - current_measurement;

	/* Integral Part */
	pid->integrator_element += 0.5f * pid->period * pid->ki * (pid->error + pid->prev_error);

	// Anti wind-up via dynamic integrator clamping
	if(pid->max_integrator > pid->proportional_element) {

		pid->max_integrator = pid->max_integrator - pid->proportional_element;

	} else {
		pid->max_integrator = 0.0f;
	}

	if(pid->min_integrator < pid->proportional_element) {

		pid->max_integrator = pid->min_integrator - pid->proportional_element;

	} else {
		pid->min_integrator = 0.0f;
	}

	if(pid->integrator_element > pid->max_integrator) {

		pid->integrator_element = pid->max_integrator;

	} else if(pid->integrator_element < pid->min_integrator) {

		pid->integrator_element = pid->min_integrator;

	}

	/* Derivative Part */
	pid->derivativation_element = (2.0f * pid->kd * (current_measurement - pid->prev_measurement)
								+ (2.0f * pid->tau - pid->period) * pid->derivativation_element)
								/ (2.0f * pid->tau + pid->period);

	/* Proportional Part */
	pid->proportional_element = pid->kp * pid->error;

	pid->output = pid->integrator_element + pid->derivativation_element + pid->proportional_element;
	if(pid->output > pid->max_output_val) {

		pid->output = pid->max_output_val;

	} else if (pid->output < pid->min_output_val){

		pid->output = pid->min_output_val;

	}

	/* store error and measurement */
	pid->prev_error = pid->error; // Update previous error for next derivative calculation
	pid->prev_measurement = current_measurement;

	return pid->output;
}
