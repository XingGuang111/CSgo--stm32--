#include "stm32f10x.h"                  // Device header
#include "Delay.h"
 
void Key_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//这里需要读取按键K1，用的上拉输入。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4||GPIO_Pin_3; 	//K1按键引脚接在了，PB4和PB11两个接口上。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
 
uint8_t Key_GetNum(void)					//右键查询：uint8_t是typedef unsigned char
{
	uint8_t KeyNum = 0;
	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)	//if判断按键1按下没有松手。
	{
		Delay_ms(20);		//消抖动
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0);		//还没有松手，在while里面循环。
		Delay_ms(20);		//松手，跳出while，延时消抖动。
		KeyNum = 1;	        //按键1返回1
	}
//	if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)   //if判断按键2按下没有松手。
//	{
//		Delay_ms(20);
//		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0);
//		Delay_ms(20);
//		KeyNum = 2;
//	}
	
	return KeyNum;	    
}
