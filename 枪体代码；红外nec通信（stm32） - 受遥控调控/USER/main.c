#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stm32f10x.h"
#include "oled.h"
#include "HW_remote.h"
#include "Send_IR.h"
#include "key.h"
#include "Motor.h"
#include "time.h"
#include "NRF24L01.h"
#define id   0x02          //////////////////////////////枪的id
u8 addr;
u8  Init=0;  
extern u8 add;
extern	u8 sta;
int xue=99;
int zd=30;
int sj=59;   //单位分钟
u16 zhanji=0;
u16 t=59;
int ttt;
u8 flag;   //开始标志位	   为1 是开始
u8 flag1,flag2,flag3;
u8 Buf[32] = {6, 0x11, 0x22, 0x33, 0x44, 0x55,0x66}; //2.4g发 
u8 Buf_j[32] = {0};                                  //2.4g收
u8  si_id;
u8 si_idd[30]={0};
u8 Buf_can;
void  seng_24g()
{
   Buf[0]=flag;
	 Buf[1]=id;
	 Buf[2]=si_id;
	 Buf[3]=xue;
	 Buf[4]=zd;
	 Buf[5]=zhanji;
}
int main()
{
    u8 re_IR;
	  u8  key;
    u8 demo[100];   //显示oled使用
		u8 demo1[100];   //显示oled使用
		u8 demo2[100];   //显示oled使用

    /* 初始化 */
    OLED_Init();
		uart1_init(9600);
			printf(" <Z>1");  //音效设置
			printf(" <V>3");  //音量设置
	   	printf(" <S>3");   //音量设置	
    OLED_Clear();
		DAT_Init();
    Remote_Init();   //红外接收初始化
		tim_Init();	

	Motor_Init();
	Motor_EN0();
NRF24L01_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
flag=1;
	NRF24L01_SendBuf(Buf);	
		seng_24g();
	NRF24L01_SendBuf(Buf);  //2.4g 信息发送
    while(1)
		{		


			      re_IR = Remote_Scan(); 						//获取命令值
				addr=re_add();										//获取地址值
				if(addr==0x10){sj= sta;flag1=1;}      											 //获取时间
				else if(addr==0x20){zd=sta;flag2=1;/*OLED_Clear();*/}					 //获取弹量
				else if(addr==0x30){xue=sta;flag3=1;}   //获取血量
			////////////////////
		if(flag1==1&&flag2==1&&flag3==1){
				flag=1;
				flag1=0;flag2=0;flag3=0;
																}
						OLED_ShowString(5, 6, demo2, 16);   //输出字符
			////////////////////
		if(flag==1)  //开始游戏
    {
		//	printf("游戏开始");
				OLED_ShowString(40,  0, "GO    ", 16);   //输出字符

			key= Key_GetNum();	  							//下次扫描自动清0
					if(key==1)       								//发射子弹
					{	
						
						if(zd)
						{
						Send_ENC_Message(id,0X11);   //地址是枪的id，子弹的命令码是0x11
						key=0;
						zd--;
							Motor_EN0();
						printf("砰");
						}
						else {printf(" 滴滴滴");}       //没有子弹

						seng_24g();
						NRF24L01_SendBuf(Buf);
				if (NRF24L01_Get_Value_Flag() == 0)
				{
					NRF24L01_GetRxBuf(Buf_j);	
				}	
					} 
        if(sta!=0)   											//接收			
        {			Delay_ms(20);		//延时消抖动
					if((sta==0x11)&&(addr!=id)){
						IR_Rad0();
						IR_BLUE0();
					xue=xue-5;
						
					Delay_ms(2);		//延时消抖动
						OLED_Clear();sta=0;
					}   		//中弹
        }	
				
				IR_Rad1();  //平时指示灯灭
				IR_BLUE1();    //平时指示灯灭

				/////判断范围
				if(zd<=0){zd=0;}
				if(xue<=0){si_id=addr;flag=3;xue=0;}
				 if(xue>=100){xue=100;}
				if(sj<=0){sj=0;flag=2;}
				
			if (NRF24L01_Get_Value_Flag() == 0)
				{
					NRF24L01_GetRxBuf(Buf_j);	
				}		
				
				 ///战绩计算
					if( (Buf_j[0]==3)&&(Buf_j[2]==id))   
				{
					if(si_idd[Buf_j[1]]==0)   //没有记录，再记录，（记录标志位）
					{
							zhanji++;
						si_idd[Buf_j[1]]=1;
					}
				
				}
			
					sprintf((char *)demo, "ID:%d", id);   //自己的信息
				sprintf((char *)demo1, " %d: %d",sj,t);	                    //比赛剩余时间
				sprintf((char *)demo2, "T:%d,D:%d,X:%d", zhanji,zd,xue);  
        OLED_ShowString(5, 2, demo, 16);   //输出字符
				OLED_ShowString(5, 4, demo1, 16);   //输出字符
				OLED_ShowString(5, 6, demo2, 16);   //输出字符
    }
		else if(flag==3)	{OLED_ShowString(40,  0, "Defeat", 16);printf(" 死亡"); 
													seng_24g();
												NRF24L01_SendBuf(Buf);  //2.4g 信息发送
								Motor_EN1();//马达震动
			}  //血量为0 死亡 游戏结束
		else if(flag==2){OLED_ShowString(20,  0, "Game over    ", 16);  printf("游戏结束"); } //时间为0  游戏结束
		else if(flag==0){ OLED_ShowString(20,  0, "waiting....", 16); }  

	}
}
void TIM2_IRQHandler(void)
{
		if( TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //1s进入一次
		{
			if(flag==1)t--;
			
				else if(flag==3)
				{
				if(++ttt>=1){
					
					
						IR_Rad0();
						IR_BLUE0();
				
				}
				else {
						IR_Rad1();
						IR_BLUE1();
						 }
				if(ttt==5){ttt=0;}
				}
		}

		if(t==0){t=60;sj--;}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		
}

