#include "HW_remote.h"
#include "delay.h"
#include "usart.h"

//红外遥控初始化
//设置IO以及定时器4的输入捕获
void Remote_Init(void)    			  
{  
	GPIO_InitTypeDef GPIO_InitStructure;  ///io口初始化
	NVIC_InitTypeDef NVIC_InitStructure;   ///中断  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  //定时器
	TIM_ICInitTypeDef  TIM_ICInitStructure;  
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE); //使能PORTB时钟 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);	//TIM4 时钟使能 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				 //PB9 输入 
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		//上拉输入 
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOB,GPIO_Pin_9);	//初始化GPIOB.9
						  
 	TIM_TimeBaseStructure.TIM_Period = 10000; //设定计数器自动重装值 最大10ms溢出  
	TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 	//预分频器,1M的计数频率,1us加1.	   	
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;  // 选择输入端 IC4映射到TI4上
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频 
  TIM_ICInitStructure.TIM_ICFilter = 0x03;//IC4F=0011 配置输入滤波器 8个定时器时钟周期滤波
  TIM_ICInit(TIM4, &TIM_ICInitStructure);//初始化定时器输入捕获通道

  TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4
 
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器	

	TIM_ITConfig( TIM4,TIM_IT_Update|TIM_IT_CC4,ENABLE);//允许更新中断 ,允许CC4IE捕获中断								 
}

//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留	
//[4]:标记上升沿是否已经被捕获								   
//[3:0]:溢出计时器
u8 	RmtSta=0;	  	  //定义一个8位的字符，8位置的含义如上
u16 Dval;		//下降沿时计数器的值
u32 RmtRec=0;	//红外接收到的数据	   		    
u8  RmtCnt=0;	//按键按下的次数	
u8 add;   //地址
//定时器4中断服务程序	 
void TIM4_IRQHandler(void)
{ 		    	
 
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)!=RESET)
	{
		if(RmtSta&0x80)								//上次有数据被接收到了  判断第8位是1进入
		{	
			RmtSta&=~0X10;							//取消上升沿已经被捕获标记  ,第5位清零
			if((RmtSta&0X0F)==0X00)                               //低4位==0000进入
				RmtSta|=1<<6;	//标记已经完成一次按键的键值信息采集   //第7位=1
			if((RmtSta&0X0F)<14)  //低4位之和小于14
				RmtSta++;
			else
			{
				RmtSta&=~(1<<7);					//清空引导标识，即将最高位置清0
				RmtSta&=0XF0;						//清空计数器	
			}								 	   	
		}							    
	}
	if(TIM_GetITStatus(TIM4,TIM_IT_CC4)!=RESET)
	{	  
		if(RDATA)																												//上升沿捕获
		{
  		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Falling);						//CC4P=1	设置为下降沿捕获
			TIM_SetCounter(TIM4,0);																				//清空定时器值
			RmtSta|=0X10;																									//标记上升沿已经被捕获  第5位 置 1
		}
		else //下降沿捕获
		{
			Dval=TIM_GetCapture4(TIM4);																//读取CCR4也可以清CC4IF标志位
  		TIM_OC4PolarityConfig(TIM4,TIM_ICPolarity_Rising);				//CC4P=0	设置为上升沿捕获
			if(RmtSta&0X10)							//完成一次高电平捕获 
			{
 				if(RmtSta&0X80)//接收到了引导码
				{
					
					if(Dval>300&&Dval<800)			//560为标准值,560us
					{
						RmtRec<<=1;					//左移一位.
						RmtRec|=0;					//接收到0	    //低电平
					}else if(Dval>1400&&Dval<1800)	//1680为标准值,1680us
					{
						RmtRec<<=1;					//左移一位.
						RmtRec|=1;					//接收到1
					}else if(Dval>2200&&Dval<2600)	//得到按键键值增加的信息 2500为标准值2.5ms
					{
						RmtCnt++; 					//按键次数增加1次
						RmtSta&=0XF0;				//清空计时器		
					}
 				}
				else if(Dval>4200&&Dval<4700)		//4500为标准值4.5ms
				{
					RmtSta|=1<<7;					//标记成功接收到了引导码
					RmtCnt=0;						//清除按键次数计数器
				}						 
			}
			RmtSta&=~(1<<4);
		}				 		     	    					   
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update|TIM_IT_CC4);	 	    
}
	u8 sta;
//处理红外键盘
//返回值:
//	 0,没有任何按键按下
//其他,按下的按键键值.
u8 Remote_Scan(void)
{        
    
	
    u8 t1,t2; 
	u8 t3,t4;
			u8 a;
	u8 t=0;
	int i;
	if(RmtSta&(1<<6))//得到一个按键的所有信息了
	{ 
	    t1=RmtRec>>24;			//得到地址码
	    t2=(RmtRec>>16)&0xff;	//得到地址反码 
 	 //   if((t1==(u8)~t2)&&t1==REMOTE_ID)//检验遥控识别码(ID)及地址
		if((t1==(u8)~t2))		//检验遥控识别码(ID)及地址
	    { 
	        t3=RmtRec>>8;
	        t4=RmtRec;
						a=t3;
	        if(t3==(u8)~t4)
					{
					for (i=0;i<8;i++)t= t | (( a>>i & 1)<< (7-i) );  //反转
							a=t;
					}
					sta=a;//键值正确	  命令码
					t=0;
					for (i=0;i<8;i++)t= t | (( t1>>i & 1)<< (7-i) );  //反转
							add=t;   //地址码
					
							
		}   
		if((sta==0)||((RmtSta&0X80)==0))//按键数据错误/遥控已经没有按下了
		{
		 	RmtSta&=~(1<<6);//清除接收到有效按键标识
			RmtCnt=0;		//清除按键次数计数器
		}
	}  
    return sta;
}
u8 re_add()
{
return add;
}




