#ifndef __Motor_H__
#define __Motor_H__
#include "stm32f10x.h"


void Motor_Init(void);

#define  Motor_EN0()		 GPIO_ResetBits(GPIOA,GPIO_Pin_0)   //¹Ø
#define  Motor_EN1()		 GPIO_SetBits(GPIOA,GPIO_Pin_0)     //¿ª

#define IR_Rad0()		 GPIO_ResetBits(GPIOB, GPIO_Pin_8)
#define IR_Rad1()		 GPIO_SetBits(GPIOB, GPIO_Pin_8)  //Ãð
#define IR_BLUE0()		 GPIO_ResetBits(GPIOB, GPIO_Pin_7)
#define IR_BLUE1()		 GPIO_SetBits(GPIOB, GPIO_Pin_7) //Ãð
#endif

