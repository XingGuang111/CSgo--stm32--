/**
  ********************************** Copyright *********************************
  *
  ** (C) Copyright 2022-2022 xxx,China.
  ** All Rights Reserved.
  *
  ******************************************************************************
  **--------------------------------------------------------------------------**
  ** @FileName      : oled.h
  ** @Description   : None
  **--------------------------------------------------------------------------**
  ** @Author        : Luo Chen
  ** @Version       : v1.0
  ** @Creat Date    : 2022-07-16
  **--------------------------------------------------------------------------**
  ** @Modfier       : None
  ** @Version       : None
  ** @Modify Date   : None
  ** @Description   : None
  **--------------------------------------------------------------------------**
  ** @Function List : None
  **--------------------------------------------------------------------------**
  ** @Attention     : None
  **--------------------------------------------------------------------------**
  ******************************************************************************
  *
 **/

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __OLED_H_
#define __OLED_H_

#include "stm32f10x.h"

#include "sys.h"
#include "stdlib.h"
//////////////////////////////////////////////////////////////////////////////////

//-----------------OLED端口定义----------------

#define OLED_SCLK_Pin		GPIO_Pin_10
#define OLED_SDIN_Pin 	    GPIO_Pin_11
#define OLED_CLK 				     RCC_APB2Periph_GPIOB
#define OLED_PORT				   GPIOB

#define OLED_SCLK_Clr() GPIO_ResetBits(OLED_PORT,OLED_SCLK_Pin)
#define OLED_SCLK_Set() GPIO_SetBits(OLED_PORT,OLED_SCLK_Pin)//SCL

#define OLED_SDIN_Clr() GPIO_ResetBits(OLED_PORT,OLED_SDIN_Pin)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(OLED_PORT,OLED_SDIN_Pin)


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据



void I2C_Start(void);
void I2C_Stop(void);
void I2C_WaitAck(void);
void Send_Byte(u8 dat);
void OLED_WR_Byte(u8 dat, u8 mode);
void Write_Cmd(void);
void OLED_WR_CMD(uint8_t cmd);
void OLED_WR_DATA(uint8_t data);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(uint8_t x, uint8_t y);
void OLED_On(void);
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2);
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size);
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no);


#endif


/******************************** END OF FILE *********************************/




