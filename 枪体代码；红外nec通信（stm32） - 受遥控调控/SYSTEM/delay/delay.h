#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//使用SysTick的普通计数模式对延迟进行管理（适合STM32F10x系列）
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.8
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//V1.3修改说明
//增加了对UCOSII延时的支持.
//如果使用ucosII,delay_init会自动设置SYSTICK的值,使之与ucos的TICKS_PER_SEC对应.
//delay_ms和delay_us也进行了针对ucos的改造.
//delay_us可以在ucos下使用,而且准确度很高,更重要的是没有占用额外的定时器.
//delay_ms在ucos下,可以当成OSTimeDly来用,在未启动ucos时,它采用delay_us实现,从而准确延时
//可以用来初始化外设,在启动了ucos之后delay_ms根据延时的长短,选择OSTimeDly实现或者delay_us实现.
//V1.4修改说明 20110929
//修改了使用ucos,但是ucos未启动的时候,delay_ms中中断无法响应的bug.
//V1.5修改说明 20120902
//在delay_us加入ucos上锁，防止由于ucos打断delay_us的执行，可能导致的延时不准。
//V1.6修改说明 20150109
//在delay_ms加入OSLockNesting判断。
//V1.7修改说明 20150319
//修改OS支持方式,以支持任意OS(不限于UCOSII和UCOSIII,理论上任意OS都可以支持)
//添加:delay_osrunning/delay_ostickspersec/delay_osintnesting三个宏定义
//添加:delay_osschedlock/delay_osschedunlock/delay_ostimedly三个函数
//V1.8修改说明 20150519
//修正UCOSIII支持时的2个bug：
//delay_tickspersec改为：delay_ostickspersec
//delay_intnesting改为：delay_osintnesting
////////////////////////////////////////////////////////////////////////////////// 
	 
#define USE_DWT_DELAY			1	/* 1、使用dwt内核精确延时  0、使用STStick定时 */

#if USE_DWT_DELAY
#define USE_TICK_DELAY		1		/* 不使用SysTick延时,使用dwt */
#else
#define USE_TICK_DELAY		0		/* 使用SysTick延时，不使用dwt */
#endif


/*简单任务管理*/
#define TASK_ENABLE 0
#define NumOfTask 3


#if USE_TICK_DELAY


#define Delay_ms(ms)  	CPU_TS_Tmr_Delay_MS(ms)
#define Delay_us(us)  	CPU_TS_Tmr_Delay_US(us)
/* 最大延时 60s=2的32次方/72000000 */
#define Delay_s(s)  	  CPU_TS_Tmr_Delay_S(s)

/* 获取内核时钟频率 */
#define GET_CPU_ClkFreq()       (SystemCoreClock)
#define SysClockFreq            (SystemCoreClock)
/* 为方便使用，在延时函数内部调用CPU_TS_TmrInit函数初始化时间戳寄存器，
   这样每次调用函数都会初始化一遍。
   把本宏值设置为0，然后在main函数刚运行时调用CPU_TS_TmrInit可避免每次都初始化 */  

#define CPU_TS_INIT_IN_DELAY_FUNCTION   1


/*******************************************************************************
 * 							函数声明
 ******************************************************************************/
uint32_t CPU_TS_TmrRd(void);
void CPU_TS_TmrInit(void);
//使用以下函数前必须先调用CPU_TS_TmrInit函数使能计数器，或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
//最大延时值为60秒

void	  CPU_TS_Tmr_Delay_US(uint32_t us);
#define CPU_TS_Tmr_Delay_MS(ms)     CPU_TS_Tmr_Delay_US(ms*1000)
#define CPU_TS_Tmr_Delay_S(s)       CPU_TS_Tmr_Delay_MS(s*1000)

#else 
//使用以下函数前必须先调用Delay_init函数使能计数器，或使能宏CPU_TS_INIT_IN_DELAY_FUNCTION
//nms<=1864
#define CPU_TS_INIT_IN_DELAY_FUNCTION   1
void Delay_init(void);

#define Delay_ms(ms)  	CPU_TS_Tmr_Delay_MS(ms)
#define Delay_us(us)  	CPU_TS_Tmr_Delay_US(us)
#define Delay_s(s)  	  CPU_TS_Tmr_Delay_S(s)

void	  CPU_TS_Tmr_Delay_US(uint32_t us);
#define CPU_TS_Tmr_Delay_MS(ms)     CPU_TS_Tmr_Delay_US(ms*1000)
#define CPU_TS_Tmr_Delay_S(s)       CPU_TS_Tmr_Delay_MS(s*1000)



#endif 
#endif /* __CORE_DELAY_H */






























