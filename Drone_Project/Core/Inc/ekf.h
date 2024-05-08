/*
 * ekf.h
 *
 *  Created on: Apr 14, 2024
 *      Author: eyalk
 */

#ifndef INC_EKF_H_
#define INC_EKF_H_



typedef struct  {
	float prediction_outcome;
	float measurement_outcome;

} ekf_t;

void EKF_Prediction(ekf_t* ekf);
void EKF_Update(ekf_t* ekf);

#endif /* INC_EKF_H_ */
