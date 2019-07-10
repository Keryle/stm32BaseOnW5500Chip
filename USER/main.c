#include "myw5500.h"
#include "stdio.h"
#include "wizchip_conf.h"
#include "w5500.h"
#include <string.h>
#include "stm32f10x.h"
#include "mcu_init.h"

void RCC_Configuration(void);		//设置系统时钟为72MHZ(这个可以根据需要改)
void NVIC_Configuration(void);		//STM32中断向量表配配置
void Timer2_Init_Config(void);		//Timer2初始化配置
void Delay(unsigned int d);			//延时函数(ms)

unsigned int Timer2_Counter=0; //Timer2定时器计数变量(ms)
unsigned int W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)


main()
{
  uint8_t sendata[] = "Hello,world!\n";
  printf("%2d\n",strlen(sendata));
  /**
   * SPI函数注册
   */
  w5500_Function_Config();

  /**
   * 系统初始化
   */
  System_Initialization();
  /**
   * w5500芯片初始化
   * @param None
   */
  w5500_ChipInit();

  while(1){
    if(W5500_Send_Delay_Counter > 1000){
      W5500_Send_Delay_Counter = 0;
      socket0_Send(sendata,strlen(sendata));
    }
  }
}


void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		Timer2_Counter++;
		W5500_Send_Delay_Counter++;
	}
}

void Delay(unsigned int d)
{
	Timer2_Counter=0;
	while(Timer2_Counter < d);
}
