/**
 * ****************************************
 * @file    myw5500.c
 * $Author  HaiJun
 * $date    2019-7-2
 * @brief   w5500二次开发函数
 * All Rights Reserved
 * ****************************************
 */

#include "myw5500.h"
#include "socket.h"
#include "stdio.h"
#include "string.h"
#define _DEBUG_
#define _SERVER_

static wiz_NetInfo gWIZNETINFO = {
                            .mac = {0x78, 0x83, 0x68, 0x88, 0x56, 0x38},
                            .ip =  {192, 168, 31,2},
                            .sn =  {255,255,255,0},
                            .gw =  {192, 168,31,1},
                            .dns = {192,168,31,1},
                            .dhcp = NETINFO_DHCP
                          };

uint8_t locall_Ip[4] = {192,168,31,2};
uint8_t destip[4] = {192,168,31,18};
uint16_t destport = 60;

void cris_en(void){}
void cris_ex(void){}
/**
 * @brief   官方接口函数配置
 * @ret     None
 */
void w5500_Function_Config(void)
{
  reg_wizchip_spi_cbfunc(SPI_ReadByte,SPI_WriteByte);
  reg_wizchip_cs_cbfunc(SPI_CS_Select,SPI_CS_Deselect);
  reg_wizchip_spiburst_cbfunc(SPI_ReadNByte,SPI_WriteNByte);
  reg_wizchip_cris_cbfunc(cris_en, cris_ex);
}

/**
 * 回读IP地址，检查是否配置正确
 */
void configNet(){
  wiz_NetInfo conf;
  // 配置网络地址
  ctlnetwork(CN_SET_NETINFO,(void *)&gWIZNETINFO);

  #ifdef _DEBUG_
  // 回读
  ctlnetwork(CN_GET_NETINFO,(void *)&conf);
  if(memcmp(&conf,&gWIZNETINFO,sizeof(wiz_NetInfo)) == 0){
    printf("IP Config successful\n");
    printf("locall_Ip = %d,%d,%d,%d\n",locall_Ip[0],locall_Ip[1],locall_Ip[2],locall_Ip[3]);
  }else{
    printf("IP Config failed\n");
  }
  #endif
}

/**
 * W5500芯片初始化,
 */
void w5500_ChipInit(void)
{
  uint32_t i=0x0000ffff;
  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}};
  w5500_Function_Config();
  wizchip_sw_reset();
  if(ctlwizchip(CW_INIT_WIZCHIP,(void*)memsize) == 0){
    #ifdef _DEBUG_
    printf("W5500 chip initialize successful\n");
    #endif
  }

  configNet();  //check IP

  //socket 0 初始化
  for(i = 0x002f00f0; i > 5; i--);
  if(socket(0,Sn_MR_TCP,5000,SF_TCP_NODELAY) == 0)    //open socket 0;
  {
    #ifdef _DEBUG_
    printf("socket 0 opened\n");
    #endif
  }

  /**
   * 服务端配置测试
   */
  #ifdef _SERVER_
  switch(listen(0)){
    case SOCK_OK: printf("SOCK_OK\n");  break;
    case SOCKERR_SOCKINIT:  printf("Socket is not initialized \n"); break;
    case SOCKERR_SOCKCLOSED:  printf("Socket closed unexpectedly.\n");  break;
    default:  printf("what the fuck?\n"); break;
  }
  #endif

  /**
    * 客户端配置测试
    */
  #ifdef _CLIENT_
  switch(connect(0,destip,destport)){
    case SOCK_OK: printf("SOCK_OK\n");break;
    case SOCKERR_TIMEOUT: printf("SOCKERR_TIMEOUT \n");break;
    case SOCKERR_IPINVALID :  printf("Wrong server IP address \n");  break;
    case SOCK_BUSY:  printf("SOCK_BUSY  \n");  break;
    case SOCKERR_SOCKNUM:   printf("SOCKNUM\n");  break;
    case SOCKERR_SOCKMODE:  printf("SOCKMODE\n"); break;
    case SOCKERR_SOCKINIT:  printf("SOCKERR_SOCKINIT\n" ); break;
    case SOCKERR_PORTZERO:  printf("Server port zero\n"); break;
    default:  printf("what the fuck?\n"); break;
  }
  #endif

  switch(getSn_SR(0)){
    case SOCK_LISTEN:  printf("SOCK_LISTEN\n");break;
    case SOCK_CLOSED:  printf("SOCK_CLOSED\n");break;
    case SOCK_INIT:    printf("SOCK_INIT\n");break;
    case SOCK_FIN_WAIT:printf("SOCK_FIN_WAIT\n");break;
    case SOCK_CLOSING: printf("SOCK_CLOSING\n");break;
    case SOCK_TIME_WAIT:  printf("SOCK_TIME_WAIT\n");break;
    default:  printf("what the fuck?\n");  break;

  }
}

void socket0_Send(uint8_t *buf, uint16_t len)
{

  switch(send(0,buf,len)){
    case 1: printf("1 byte data sent Success\n");  break;
    case 13: printf("13 byte data sent Success\n"); break;
    case SOCKERR_SOCKSTATUS: printf(" Invalid socket status for socket operation \n"); break;
    case SOCKERR_TIMEOUT: printf("Timeout occurred \n"); break;
    case SOCKERR_SOCKMODE:  printf("Invalid operation in the socket \n"); break;
    case SOCK_BUSY: printf("Socket is busy\n"); break;
  }
}
