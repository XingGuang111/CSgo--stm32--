#ifndef __Send_IR_H__
#define __Send_IR_H__
#include "stm32f10x.h"
#include "delay.h"

void DAT_Init(void);
void Send_IR(unsigned int i);
void NoSend_IR(unsigned int i);
void Send_NEC_0(void);
void Send_NEC_1(void); 
void GetByte_SendByte(unsigned char Cmd);
void Send_ENC_Message(unsigned char user_code,unsigned char Cmd);
void GetByte_And_SendByte(unsigned char user_code,unsigned char Cmd);
//ºìÍâ¿ØÖÆÒý½Å

#define IR_EN		GPIO_Pin_3
#define IR_EN0()		 GPIO_ResetBits(GPIOA,IR_EN)
#define IR_EN1()		 GPIO_SetBits(GPIOA,IR_EN)
//Send_ENC_Message(0x00,0x08);

#endif

