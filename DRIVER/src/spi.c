/**
 * ******************************************
 * @file    spi.c
 * $Author: HaiJun
 * $data:   2019-7-1
 * @brief   spi函数的实现
 * All Rights Reserved
 * ******************************************
 */

#include "spi.h"

/**
 * 初始化SPI口
 * SCK->PA5,MISO->PA6,MOSI->PA7,CS->PA4
 */
void SPI_Configuration(void)
{
  GPIO_InitTypeDef 	GPIO_InitStructure;
  SPI_InitTypeDef   SPI_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO, ENABLE);

  GPIO_InitStructure.GPIO_Pin = W5500_SCK | W5500_MISO | W5500_MOSI;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(W5500_SPI_PORT, &GPIO_InitStructure);
	GPIO_SetBits(W5500_SPI_PORT,W5500_SCK | W5500_MISO | W5500_MOSI);

  /* 初始化cs引脚 */
  GPIO_InitStructure.GPIO_Pin = W5500_SCS;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(W5500_SPI_PORT, &GPIO_InitStructure);
	GPIO_SetBits(W5500_SPI_PORT, W5500_SCS);

  /* 初始化配置STM32 SPI1 */
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;	//SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;							//设置为主SPI
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;					//SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;							  //时钟悬空低
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_1Edge;							//数据捕获于第1个时钟沿
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;							   	//NSS由外部管脚管理
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;	//波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;				//数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial=7;							   	//CRC多项式为7
	SPI_Init(SPI1,&SPI_InitStructure);							    		//根据SPI_InitStruct中指定的参数初始化外设SPI1寄存器

SPI_Cmd(SPI1,ENABLE);	//STM32使能SPI1
}

/**
 * @brief   写一个字节到SPI总线
 * @param   TxData
 * @ret     None
 */
 void SPI_WriteByte(uint8_t TxData)
 {
   SPI_I2S_SendData(SPI1,TxData);//写1个字节数据
   while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);//等待数据寄存器空
 }

void SPI_WriteNByte(uint8_t *pdata, uint16_t len)
{
  uint16_t i;
  for(i = 0; i < len; i++)
    SPI_WriteByte(*pdata++);
}

 /**
  * @brief  从SPI总线读取1字节数据
  * @ret    读到的数据
  */
uint8_t SPI_ReadByte(void)
{
  SPI_I2S_ReceiveData(SPI1);
  SPI_WriteByte(0x00);//发送一个哑数据
  return SPI_I2S_ReceiveData(SPI1);//读取1个字节数据
}

 void SPI_ReadNByte(uint8_t* pBuf, uint16_t len) 	{
   for(;len > 0;len--){
     *pBuf++ = SPI_ReadByte();
   }
 }

 /**
  * @brief  片选信号输出低电平
  * @ret    None
  */
void SPI_CS_Select(void)
{
    GPIO_ResetBits(GPIOA,W5500_SCS);
}

/**
  * @brief 片选信号输出高电平
  * @ret   None
  */
void SPI_CS_Deselect(void)
{
    GPIO_SetBits(GPIOA,W5500_SCS);
}

/* ******************** end ********************** */
