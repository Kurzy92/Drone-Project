/*
 * lsm6dsox.c
 *
 *  Created on: Mar 26, 2024
 *      Author: eyalk
 */


#include "lsm6dsox.h"





uint8_t LSM6DSOX_I2C_init(LSM6DSOX_I2C *dev, I2C_HandleTypeDef *pHI2C) {
	// Set dev struct
	dev->i2cHandle = pHI2C;
	dev->acc_data_mps2[0] = 0.0f;
	dev->acc_data_mps2[1] = 0.0f;
	dev->acc_data_mps2[2] = 0.0f;
	dev->angular_rate_dps[0] = 0.0f;
	dev->angular_rate_dps[1] = 0.0f;
	dev->angular_rate_dps[2] = 0.0f;
	dev->temp_c = 0.0f;

	HAL_StatusTypeDef status;
	uint8_t regdata;

	status = LSM6DSOX_I2C_ReadRegister(dev, LSM6DSOX_REG_WHO_AM_I, &regdata);
	if(status != HAL_OK || regdata != LSM6DSOX_WHO_AM_I_VAL) {
		//printf("Device address doesn't match");
		return 255;
	}

	/*   Configuring Accelerometer Sensor   */
	//CTRL1_XL
	regdata = 0;
	regdata |= LSM6DSOX_CTRL1_XL_SCALE_4G;
	regdata |= LSM6DSOX_CTRL1_XL_SPEED_416HZ;
	status = LSM6DSOX_I2C_WriteRegister(dev, LSM6DSOX_REG_CTRL1_XL,&regdata);

	dev->acc_full_scale_range = 4;
	dev->acc_scale_resolution = 1.0/32768.0;
	dev->acc_FS_to_scale = dev->acc_full_scale_range*dev->acc_scale_resolution;

	/*    Configuring Gyroscope Sensor   */
	regdata = 0;
	//CTRL2_G
	regdata |= LSM6DSOX_CTRL2_G_FS_500;
	regdata |= LSM6DSOX_CTRL2_G_ODR_416HZ;
	status = LSM6DSOX_I2C_WriteRegister(dev, LSM6DSOX_REG_CTRL2_G, &regdata);

	status = LSM6DSOX_I2C_ReadRegister(dev, LSM6DSOX_REG_CTRL2_G, &regdata);

	dev->angular_rate_sensitivity = 500;
	dev->angular_mdps_to_scale = dev->angular_rate_sensitivity * dev->acc_scale_resolution;
	return 0;
}


/*
 * Data Acquisition
 */

/**
 * @brief This function reads the temperature measured by the sensor.
 *
 * It utilizes I2C communication to read the temperature sensor data.
 * I2C communication is done using STM32 HAL I2C API.
 *
 * @param dev 	A pointer to an LSM6DSOX_I2C structure representing the device from which
 * 				the temperature is read. The temperature value is written into the
 * 				temp_c member of the LSM6DSOX_I2C struct
 *
 * @return 	Returns a HAL_Status_TypeDef enumerate value that indicates the operation success or error.
 * 			If the function returns 'HAL_OK', it indicates that the temperature was successfully read
 * 			and the value is available in 'dev->temp_c'.
 * 			If 'HAL_OK' isn't the returned value, the function hasn't completed successfully
 */
HAL_StatusTypeDef LSM6DSOX_I2C_ReadTemp(LSM6DSOX_I2C *dev) {
	uint8_t regData = 0;
	int16_t getTemp = 0;
	HAL_StatusTypeDef status = LSM6DSOX_I2C_ReadRegister(dev, LSM6DSOX_REG_OUT_TEMP_H, &regData);
	if(status != HAL_OK) {
		return HAL_ERROR;
	}
	getTemp |= (regData << 8);
	status = LSM6DSOX_I2C_ReadRegister(dev, LSM6DSOX_REG_OUT_TEMP_L, &regData);
	getTemp |= regData;
	dev->temp_c = LSM6DSOX_CONST_TEMP_VAL_AT_0_LSB + LSM6DSOX_CONST_TEMP_SENSITIVITY_C_TO_LSB * (float)getTemp - 3;
	return HAL_OK;
}


/**
 * @brief This function reads measurements made by the accelerometer.
 *
 * It utilizes I2C communication to read the accelerometer sensor data
 * for 3 axes(x,y,z in the drone's body frame)
 *
 * I2C communication is done using STM32 HAL I2C API.
 *
 * @param dev 	A pointer to an LSM6DSOX_I2C structure representing the device from which
 * 				the accelerometer data is read. The accelerometer value is written into the
 * 				acc_data_msp2[x,y,z] member of the LSM6DSOX_I2C struct
 *
 * @return 	Returns a HAL_Status_TypeDef enumerate value that indicates the operation success or error.
 * 			If the function returns 'HAL_OK', it indicates that the accelerometer data was successfully read
 * 			and the value is available in 'dev->acc_data_msp2'.
 * 			If 'HAL_OK' isn't the returned value, the function hasn't completed successfully
 */
HAL_StatusTypeDef LSM6DSOX_I2C_ReadAccel(LSM6DSOX_I2C *dev) {
	uint8_t regdata[6];
	// Read all Accelerometer measurements for axes x,y,z.
	HAL_StatusTypeDef status = LSM6DSOX_I2C_ReadRegisters(dev, LSM6DSOX_REG_OUTX_L_A, regdata, 6);
	if(status != HAL_OK)
		return HAL_ERROR;
	int16_t AccData[3];
	AccData[0] = ((int16_t)regdata[1] << 8) | ((int16_t)regdata[0]); // X-axis
	AccData[1] = ((int16_t)regdata[3] << 8) | ((int16_t)regdata[2]); // Y-axis
	AccData[2] = ((int16_t)regdata[5] << 8) | ((int16_t)regdata[4]); // Z-axis

	// Convert raw data to meters per second^2
	dev->acc_data_mps2[0] = AccData[0] * dev->acc_FS_to_scale * G_ACC + LSM6DSOX_GYRO_CALIB_CONST_X;
	dev->acc_data_mps2[1] = AccData[1] * dev->acc_FS_to_scale * G_ACC + LSM6DSOX_GYRO_CALIB_CONST_Y;
	dev->acc_data_mps2[2] = AccData[2] * dev->acc_FS_to_scale * G_ACC + LSM6DSOX_GYRO_CALIB_CONST_Z;

	return HAL_OK;
}


HAL_StatusTypeDef LSM6DSOX_I2C_ReadGyro(LSM6DSOX_I2C *dev) {
	uint8_t regdata[6];
	// Read all Gyroscope measurements for axes x,y,z.
	HAL_StatusTypeDef status = LSM6DSOX_I2C_ReadRegisters(dev, LSM6DSOX_REG_OUTX_L_G, regdata, 6);
	if(status != HAL_OK)
		return HAL_ERROR;
	int16_t GyroData[3];
	GyroData[0] = ((int16_t)regdata[1] << 8) | ((int16_t)regdata[0]); // X-axis
	GyroData[1] = ((int16_t)regdata[3] << 8) | ((int16_t)regdata[2]); // Y-axis
	GyroData[2] = ((int16_t)regdata[5] << 8) | ((int16_t)regdata[4]); // Z-axis

	// Convert raw data to dps
	dev->angular_rate_dps[0] = GyroData[0] * GYRO_DPS_TO_LSB;
	dev->angular_rate_dps[1] = GyroData[1] * GYRO_DPS_TO_LSB;
	dev->angular_rate_dps[2] = GyroData[2] * GYRO_DPS_TO_LSB;

	return HAL_OK;
}



/*
 * Register Operations Functions
 */


HAL_StatusTypeDef LSM6DSOX_I2C_ReadRegister(LSM6DSOX_I2C *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LSM6DSOX_I2C_ReadRegisters(LSM6DSOX_I2C *dev, uint8_t reg, uint8_t *data, uint8_t length) {
	return HAL_I2C_Mem_Read(dev->i2cHandle, LSM6DSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LSM6DSOX_I2C_WriteRegister(LSM6DSOX_I2C *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Write(dev->i2cHandle, LSM6DSOX_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

