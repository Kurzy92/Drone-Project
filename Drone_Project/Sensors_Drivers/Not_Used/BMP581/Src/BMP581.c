/*
 * BMP581.c
 *
 *  Created on: May 4, 2024
 *      Author: eyalk
 */

#include "BMP581.h"

void BMP581_Init(BMP581_t* bar, void*  pHandler, uint8_t isSPI , GPIO_TypeDef * pSPI_CS_PORT, uint8_t pSPI_CS_PIN) {
	if(isSPI) {
		bar->pHSPI = (SPI_HandleTypeDef*)pHandler;
		bar->pSPI_GPIO_PIN = pSPI_CS_PIN;
		bar->pSPI_GPIO_PORT = pSPI_CS_PORT;
		bar->pHI2C = NULL;
		bar->bSPI_p = TRUE;
	} else {
		bar->pHI2C = (I2C_HandleTypeDef*)pHandler;
		bar->pHSPI = NULL;
		bar->bSPI_p = FALSE;
	}
	bar->dummyByte = dummyByteVal;
	uint8_t dataT = BMP581_ADR_CHIP_ID;
	uint8_t dataR = 0;

	// Dummy read before starting
	BMP581_read(bar, dataT, &dataR, 1);
	dataR =0;


	dataT = BMP581_ADR_CMD;
	uint8_t dataW = BMP581_CMD_COMMAND_RESET;

	// Soft Reset
	if(BMP581_write(bar, &dataT, &dataW) != TRUE)
		return;

	dataT = BMP581_ADR_INT_STATUS;

	do {
		dataR = 0;
		if(BMP581_read(bar, dataT, &dataR, 1) != TRUE) {
			return FALSE;
		}
		dataR &= BMP581_INT_STATUS_POR_POS;
	} while(dataR  != 0x10);


	// Check BMP581's Chip ID
	if(BMP581_write_settings(bar) == TRUE) {
		bar->bInitSuccess = TRUE;
	} else {
		bar->bInitSuccess  = FALSE;
	}
	BMP581_post_powerup_procedurce(bar);
	return;
}

float BMP581_get_pressure(BMP581_t* bar) {
	if(bar->bInitSuccess != TRUE)
		return -1;
	uint8_t data = BMP581_ADR_PRESS_DATA_XLSB;
	BMP581_SPI_read_multiple_regs(bar, &data, bar->pressureRegs, 3);
	int64_t pressure = 0;
	pressure = (int32_t)((int32_t)bar->pressureRegs[0] | (int32_t)bar->pressureRegs[1] << 8 | (int32_t)bar->pressureRegs[2] << 16);
	pressure = (float)(pressure / 64);
//	return pressure;
}

static bool BMP581_write_settings(BMP581_t* bar) {
	uint8_t dataAdr= 0;
	uint8_t dataSend= 0;
	// Set OSR_CONFIG settings
	dataAdr= BMP581_ADR_OSR_CONFIG;
	dataSend = BMP581_OSR_CONFIG_PRESS_EN |
			BMP581_OSR_CONFIG_OSR_P_32X |
			BMP581_OSR_CONFIG_OSR_T_2X;
	BMP581_write(bar, &dataAdr, &dataSend);
	HAL_Delay(30);
	// Set DSP_IIR settings
	dataAdr= BMP581_ADR_DSP_IIR;
	dataSend = BMP581_DSP_IIR_SET_IIR_P_7_COEF |
			BMP581_DSP_IIR_SET_IIR_T_7_COEF;
	BMP581_write(bar, &dataAdr, &dataSend);
	HAL_Delay(30);

	// Set DSP_CONFIG settings
	dataAdr= BMP581_ADR_DSP_CONFIG;
	dataSend = BMP581_DSP_CONFIG_IIR_FLUSHED_FORCED_EN_DISABLE  |
			BMP581_DSP_CONFIG_SHDW_SEL_IIR_T_AFTER |
			BMP581_DSP_CONFIG_SHDW_SEL_IIR_P_AFTER;
	BMP581_write(bar, &dataAdr, &dataSend);
	HAL_Delay(30);

	// Set ODR_CONFIG settings
	dataAdr = BMP581_ADR_ODR_CONFIG;
	dataSend =  BMP581_ODR_CONFIG_PWR_MODE_NORMAL |
			BMP581_ODR_CONFIG_ODR_50_HZ;
	BMP581_write(bar, &dataAdr, &dataSend);
	HAL_Delay(30);
	return TRUE;
}


static bool BMP581_post_powerup_procedurce(BMP581_t* bar) {
	uint8_t data = 0;

	do {
			if(BMP581_read(bar, BMP581_ADR_INT_STATUS, &data, 1) != TRUE) {
				return FALSE;
			}
			data &= BMP581_INT_STATUS_POR_POS;
		} while(data  != 0x10);

	// CHIP ID check
	if(BMP581_read(bar, BMP581_ADR_CHIP_ID, &data, 1) != TRUE) {
		return FALSE;
	}
	if(data !=  BMP581_CHIP_ID)
		return FALSE;

	data  =0;
	// Check STATUS register
	if(BMP581_read(bar, BMP581_ADR_STATUS, &data, 1) != TRUE) {
		return FALSE;
	}
	if(data | BMP581_STATUS_STATUS_NVM_ERR_POS != 0)
		return FALSE;
	if(data & BMP581_STATUS_STATUS_NVM_READY_POS == 0)
		return FALSE;

	data =0;
	// Check INT_STATUS por register.

	return TRUE;
}


static bool BMP581_read(BMP581_t* bar, uint8_t dataAdr, uint8_t* dataRead, uint32_t length) {
	if(length == 0) {
		// Why even get here?
		return FALSE;
	}
	if(bar->bSPI_p) {
		// Do SPI reading
		if(length == 1) {
			BMP581_SPI_read_reg(bar, dataAdr, dataRead);
		} else {
			BMP581_SPI_read_multiple_regs(bar, dataAdr, dataRead, length);
		}
	} else {
		//Do I2C reading
	}
	return TRUE;
}


static bool BMP581_write(BMP581_t* bar, uint8_t* dataAdr, uint8_t* dataRead) {
	if(bar->bSPI_p) {
		// Do SPI writing
		BMP581_SPI_write_reg(bar, dataAdr, dataRead);
	} else {
		//Do I2C writing
	}
	return TRUE;
}


static HAL_StatusTypeDef BMP581_SPI_write_reg(BMP581_t* bar, uint8_t *dataReg, uint8_t* dataToWrite) {
	uint8_t buf[2];
	buf[0] = *dataReg & BMP581_SPI_WRITE_MASK;
	buf[1] = *dataToWrite;
	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_RESET);
	if(HAL_SPI_Transmit(bar->pHSPI, &buf[0], 1, 100)!= HAL_OK)
		return HAL_ERROR;
	if(HAL_SPI_Transmit(bar->pHSPI, &buf[1], 1, 100) != HAL_OK)
		return HAL_ERROR;
	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_SET);
	return HAL_OK;
}


static HAL_StatusTypeDef BMP581_SPI_read_reg(BMP581_t* bar, uint8_t *dataT, uint8_t *dataR) {
	uint8_t dataTRead = *dataT;
	dataTRead |= BMP581_SPI_READ_MASK;

	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_RESET);

	HAL_SPI_Transmit(bar->pHSPI, &dataTRead, 1, 100);
	HAL_SPI_Transmit(bar->pHSPI, &bar->dummyByte, 1, 100);
	if(HAL_SPI_Receive(bar->pHSPI, dataR, 1, 100) != HAL_OK)
		return HAL_ERROR;
	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_SET);

	return HAL_OK;
}


static HAL_StatusTypeDef BMP581_SPI_read_multiple_regs(BMP581_t* bar, uint8_t *dataT, uint8_t *dataR, uint8_t length) {
	uint8_t dataTRead = *dataT | BMP581_SPI_READ_MASK;

	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_RESET);
	HAL_SPI_Transmit(bar->pHSPI, &dataTRead, 1, 100);
	HAL_SPI_Transmit(bar->pHSPI, &bar->dummyByte, 1, 100);
	if(HAL_SPI_Receive(bar->pHSPI, dataR, length, 100) != HAL_OK)
		return HAL_ERROR;
	HAL_GPIO_WritePin(bar->pSPI_GPIO_PORT, bar->pSPI_GPIO_PIN, GPIO_PIN_SET);
	return HAL_OK;
}
