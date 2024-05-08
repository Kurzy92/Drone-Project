/*
 * lis3mdl.c
 *
 *  Created on: Apr 14, 2024
 *      Author: eyalk
 */
#include "lis3mdl.h"

HAL_StatusTypeDef LIS3MDL_Init(LIS3MDL_I2C* dev, I2C_HandleTypeDef* pHI2C) {
	dev->i2cHandle = pHI2C;

	uint8_t regdata = 0;
	LIS3MDL_I2C_ReadRegister(dev, LIS3MDL_WHO_AM_I_ADDR, &regdata);
	if(regdata != LIS3MDL_WHO_AM_I_VAL)
		return HAL_ERROR;

	/* CTRL_REG1 Configuration */
	regdata |= LIS3MDL_CTRL1_REG_DO_80HZ;
	regdata |= LIS3MDL_CTRL1_REG_TEMP_EN_ENABLE;
	LIS3MDL_I2C_WriteRegister(dev, LIS3MDL_REG_CTRL_REG1, &regdata);

	regdata = 0;
	/* CTRL_REG2 Configuration */
	regdata |= LIS3MDL_CTRL2_REG_FS_8_GAUSS;
	LIS3MDL_I2C_WriteRegister(dev, LIS3MDL_REG_CTRL_REG2, &regdata);

	regdata = 0;
	/* CTRL_REG3 Configuration */
	regdata |= LIS3MDL_CTRL3_REG_MD_CONTINUOUS_MODE;
	LIS3MDL_I2C_WriteRegister(dev, LIS3MDL_REG_CTRL_REG3, &regdata);

	return HAL_OK;
}




/*
 * Register Operations Functions
 */


HAL_StatusTypeDef LIS3MDL_I2C_ReadRegister(LIS3MDL_I2C *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Read(dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LIS3MDL_I2C_ReadRegisters(LIS3MDL_I2C *dev, uint8_t reg, uint8_t *data, uint8_t length) {
	return HAL_I2C_Mem_Read(dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY);
}


HAL_StatusTypeDef LIS3MDL_I2C_WriteRegister(LIS3MDL_I2C *dev, uint8_t reg, uint8_t *data) {
	return HAL_I2C_Mem_Write(dev->i2cHandle, LIS3MDL_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}
