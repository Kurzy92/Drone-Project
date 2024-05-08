/*
 * BMP581.h
 *
 *  Created on: May 4, 2024
 *      Author: eyalk
 */

#ifndef BMP581_INC_BMP581_H_
#define BMP581_INC_BMP581_H_

#include "BMP581_def.h"

#define BMP581_SPI_WRITE_MASK  		0x7F  // Mask to write data
#define BMP581_SPI_READ_MASK   			0x80  // Mask to read data

void BMP581_Init(BMP581_t* bar, void*  pHandler, uint8_t isSPI, GPIO_TypeDef * pSPI_CS_PORT, uint8_t pSPI_CS_PIN);

float BMP581_get_pressure(BMP581_t *bar);

static bool BMP581_write_settings(BMP581_t* bar);

static bool BMP581_read(BMP581_t* bar, uint8_t dataAdr, uint8_t* daraRead, uint32_t length);
static bool BMP581_write(BMP581_t* bar, uint8_t* dataAdr, uint8_t* daraRead);
static bool BMP581_post_powerup_procedurce(BMP581_t* bar);

static HAL_StatusTypeDef BMP581_SPI_read_reg(BMP581_t* bar, uint8_t *dataT, uint8_t *dataR);
static HAL_StatusTypeDef BMP581_SPI_read_multiple_regs(BMP581_t* bar, uint8_t *dataT, uint8_t *dataR, uint8_t length);
static HAL_StatusTypeDef BMP581_SPI_write_reg(BMP581_t* bar, uint8_t *dataReg, uint8_t* dataToWrite);

#endif /* BMP581_INC_BMP581_H_ */
