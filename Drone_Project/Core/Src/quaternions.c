/*
 * quaternions.c
 *
 *  Created on: Apr 28, 2024
 *      Author: eyalk
 */


#include "quaternions.h"

uint8_t quaternion_init(quaternion_t* q1, const float *data) {
	q1->q0 = data[0];
	q1->q1 = data[1];
	q1->q2 = data[2];
	q1->q3 = data[3];
	return 0;
}

uint8_t quaternion_addition(const quaternion_t* q1, const quaternion_t* q2, quaternion_t* q3) {
	float newQuaternion[4];
	newQuaternion[0] = q1->q0 + q2->q0;
	newQuaternion[1] = q1->q1 + q2->q1;
	newQuaternion[2] = q1->q2 + q2->q2;
	newQuaternion[3] = q1->q3 + q2->q3;
	quaternion_init(q3, newQuaternion);
	return 0;
}

uint8_t quaternion_subtraction(const quaternion_t* q1, const quaternion_t* q2, quaternion_t* q3) {
	float newQuaternion[4];
	newQuaternion[0] = q1->q0 - q2->q0;
	newQuaternion[1] = q1->q1 - q2->q1;
	newQuaternion[2] = q1->q2 - q2->q2;
	newQuaternion[3] = q1->q3 - q2->q3;
	quaternion_init(q3, newQuaternion);
	return 0;
}

uint8_t quaternion_multiplication(const quaternion_t* q1, const quaternion_t* q2, quaternion_t* q3) {
	float newQuaternion[4];
	newQuaternion[0] = (q1->q0 * q2->q0) - (q1->q1 * q2->q1) - (q1->q2 * q2->q2) - (q1->q3 * q2->q3);
	newQuaternion[1] = (q2->q0 * q1->q1) + (q2->q1 * q1->q0) - (q2->q2 * q1->q3) + (q2->q3 * q1->q2);
	newQuaternion[2] = (q2->q0 * q1->q2) + (q2->q1 * q1->q3) - (q2->q2 * q1->q0) + (q2->q3 * q1->q1);
	newQuaternion[3] = (q2->q0 * q1->q3) + (q2->q1 * q1->q2) - (q2->q2 * q1->q1) + (q2->q3 * q1->q0);
	quaternion_init(q3, newQuaternion);
	return 0;
}

uint8_t quaternion_conj(const quaternion_t * q1, quaternion_t* q1_conj) {
	float newQuaternion[4];
	newQuaternion[0] = q1->q0;
	newQuaternion[1] = -1 * q1->q1;
	newQuaternion[2] = -1 * q1->q2;
	newQuaternion[3] = -1 * q1->q3;
	quaternion_init(q1_conj, newQuaternion);
	return 0;
}

float32_t quaternion_norm(const quaternion_t* q1) {
	float32_t normCalc = 0;
	normCalc = q1->q0 * q1->q0 + q1->q1 * q1->q1 + q1->q2 * q1->q2 + q1->q3 * q1->q3;
	arm_sqrt_f32(normCalc, &normCalc);
	return normCalc;
}

uint8_t quaternion_inverse(const quaternion_t* q1, quaternion_t* q1_inv) {
	quaternion_t qConj;
	float32_t data[4];
	float32_t norm = quaternion_norm(q1);
	if(norm == 0.0f)
		return 255;
	quaternion_conj(q1, &qConj);
	for(int i = 0; i<4; i++) {
		data[i] = qConj.qData[i]/norm;
	}
	quaternion_init(q1_inv, data);
	return 0;
}

uint8_t quaternion_eulerXYZ_conversion(const quaternion_t* q, euler_t* euler) {
	float32_t q0 = q->q0;
	float32_t q1 = q->q1;
	float32_t q2 = q->q2;
	float32_t q3 = q->q3;

	float32_t phi = 0; float32_t theta = 0; float32_t  psi = 0;
	if(arm_atan2_f32(2*q2*q3+2*q0*q1, q0*q0 - q1*q1 - q2*q2 + q3*q3, &phi) != ARM_MATH_SUCCESS)
		return 255;
	theta = -1*asinf(2*q1*q3-2*q0*q2);
	if(arm_atan2_f32(2*q2*q1+2*q0*q3, q0*q0 + q1*q1 - q2*q2 - q3*q3, &psi) != ARM_MATH_SUCCESS)
		return 255;
	euler->phi = phi;
	euler->theta = theta;
	euler->psi = psi;
	return 0;

}

