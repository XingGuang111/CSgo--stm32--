#include "stm32f10x.h"                  // Device header
 
void Motor_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//这里需要读取按键K1，用的上拉输入。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 	//K1按键引脚接在了，PB4和PB11两个接口上。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//这里需要读取按键K1，用的上拉输入。
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_8; 	//K1按键引脚接在了，PB4和PB11两个接口上。
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
#define IR_EN0()		 GPIO_ResetBits(GPIOA,IR_EN)
#define IR_EN1()		 GPIO_SetBits(GPIOA,IR_EN)


