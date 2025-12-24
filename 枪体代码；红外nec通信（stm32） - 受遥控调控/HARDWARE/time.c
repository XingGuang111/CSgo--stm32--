#include "time.h"
#include "stm32f10x.h"

void tim_Init(void)
{
		TIM_TimeBaseInitTypeDef timInitStructure;
		NVIC_InitTypeDef 				nvicInitStructure;
	
		//1.配置定时器时钟
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
		//2.配置定时器结构体
		timInitStructure.TIM_ClockDivision	= TIM_CKD_DIV1;
		timInitStructure.TIM_CounterMode	= TIM_CounterMode_Up;
		timInitStructure.TIM_Period			= 10000-1;
		timInitStructure.TIM_Prescaler		= 7200-1;
		
		TIM_TimeBaseInit(TIM2, &timInitStructure);
		//3.开启定时器中断
		TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
		TIM_Cmd(TIM2, ENABLE);
		
		//4.配置中断结构体
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
		
		nvicInitStructure.NVIC_IRQChannel					= TIM2_IRQn;
		nvicInitStructure.NVIC_IRQChannelPreemptionPriority	= 1;
		nvicInitStructure.NVIC_IRQChannelSubPriority		= 1;
		nvicInitStructure.NVIC_IRQChannelCmd				= ENABLE;
		
		NVIC_Init(&nvicInitStructure);
		
		//5.搭建定时器中断服务函数
		
}

