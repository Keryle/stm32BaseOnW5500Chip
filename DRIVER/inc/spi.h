/**
 * ******************************************
 * @file    spi.h
 * $Author: HaiJun
 * $date:   2019-7-1
 * @brief   spi函数的实现
 * All Rights Reserved
 * ******************************************
 */

#ifndef _SPI_H_
#define _SPI_H_

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"

/* 定义w5500的引脚 */
#define W5500_SCS		GPIO_Pin_4
#define W5500_SCK   GPIO_Pin_5
#define W5500_MISO  GPIO_Pin_6
#define W5500_MOSI  GPIO_Pin_7
#define W5500_SPI_PORT	GPIOA


#define W5500_RST		GPIO_Pin_5
#define W5500_RST_PORT	GPIOC

#define W5500_INT		GPIO_Pin_4
#define W5500_INT_PORT	GPIOC


extern void SPI_Configuration(void);
extern void SPI_WriteByte(uint8_t TxData);
extern uint8_t SPI_ReadByte(void);
extern void SPI_CS_Select(void);
extern void SPI_CS_Deselect(void);
extern void SPI_ReadNByte(uint8_t* pBuf, uint16_t len);
extern void SPI_WriteNByte(uint8_t *pdata, uint16_t len);

#endif
