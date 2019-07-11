/**
 * ****************************************
 * @file    myw5500.h
 * $Author  HaiJun
 * $date    2019-7-2
 * @brief   w5500二次开发函数
 * All Rights Reserved
 * ****************************************
 */

#ifndef _MYW5500_H_
#define _MYW5500_H_

#include "w5500.h"
#include "wizchip_conf.h"
#include "spi.h"

extern void w5500_Function_Config(void);
extern void w5500_ChipInit_TCP(void);
extern void socket0_Send(uint8_t *buf, uint16_t len);
#endif
