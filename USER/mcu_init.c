#include "mcu_init.h"

/**
 * Set system clock to 72MHZ
 * Enable peripheral clocks
 */
void RCC_Configuration(void)
{
  ErrorStatus HSEStartUpStatus;
  RCC_DeInit();     //init
  RCC_HSEConfig(RCC_HSE_ON);    //turn on HSE
  HSEStartUpStatus = RCC_WaitForHSEStartUp();   //wait for stable HSE
  if(SUCCESS == HSEStartUpStatus){
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //AHB clock divided 1
    RCC_PCLK2Config(RCC_HCLK_Div1);   //APB2 clock divided 1
    RCC_PCLK1Config(RCC_HCLK_Div2);   //APB1 clock divided 2
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);  //HSE clock divided 1,PLLMul is 9
    RCC_PLLCmd(ENABLE);   //enable PLL
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET); //wait for stable PLL
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);  // set the clock source of system to PLL
    while(RCC_GetSYSCLKSource() != 0x08);   // wait for clock changed
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB
					| RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD
					| RCC_APB2Periph_AFIO, ENABLE);       //Enable peripheral clocks
  }
}

/**
 * NVIC_Configuration for TIM2
 */
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 * Timer2 init configuration
 */
void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//enable timer2 clock

  /*
   * time = (Period+1) * (Prescaler+1) / (system clock)
   * (9+1) * (7199+1) / 72 000 000 = 1000 um = 1ms
   */
	TIM_TimeBaseStructure.TIM_Period = 9;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 	//enable TIM2 interruput

	TIM_Cmd(TIM2, ENABLE);
}

///////////////////////////////////
//     System_Initialization     //
///////////////////////////////////
void System_Initialization(void)
{
	RCC_Configuration();
  NVIC_Configuration();
	SPI_Configuration();
	Timer2_Init_Config();
}
