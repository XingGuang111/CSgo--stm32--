#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>

//在gun或者定义#pragma anon_unions的情况下，才支持匿名联合
#if defined ( __CC_ARM   )
#pragma anon_unions
#endif


/******************************* 宏定义 ***************************/
#define            macNVIC_PriorityGroup_x                     NVIC_PriorityGroup_2

#define USART1_REC_LEN  			200  	//定义最大接收字节数 200

#define Open_redirection  1   //是否开启printf重定向
#define MicroLIB  				1		//是否开启MicroLIB
#define USARTx						USART1		//重定向的UART口

#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收


#define RX_BUF_MAX_LEN     1024                                     //最大接收缓存字节数

//接收状态
/********************************************************************************
*    同USART_RX_STA接收状态寄存器
		 bit15，	  接收完成标志
		 bit14，	  接收到0x0d
		 bit13~0，	接收到的有效字节数目
********************************************************************************/
extern struct  STRUCT_USARTx_Fram                                  //串口数据帧的处理结构体
{
    //STRUCT_USARTx_Fram 读取的数据
    char  Data_RX_BUF [ RX_BUF_MAX_LEN ];

    union {
        __IO u16 InfAll;
        struct {
            __IO u16 FramLength       :15;                               // 14:0
            __IO u16 FramFinishFlag   :1;                                // 15
        } InfBit;
    };

}strUSART_Fram_Record;

//如果想串口中断接收，请不要注释以下宏定义
void uart1_init(u32 bound);


/********************************** 函数声明 ***************************************/
void USART_printf   ( USART_TypeDef * pUSARTx, char * Data, ... );
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);
void Usart_SendString( USART_TypeDef * pUSARTx, char *str);
void Usart_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch);


#endif


