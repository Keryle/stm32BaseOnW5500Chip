#ifndef __TIMER_H_
#define __TIMER_H_

#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_it.h"
#include "misc.h"

extern __IO uint32_t TimingDelay;
void systick_init(void);
void timingDelay_decrement(void);
void delay_ms(__IO uint32_t nTime);//�ӳٺ���������Ϊ US

#endif
