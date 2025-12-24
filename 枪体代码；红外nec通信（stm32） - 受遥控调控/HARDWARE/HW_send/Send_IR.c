#include "Send_IR.h"
/*
 *发送一帧数据
*/
void DAT_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;  ///io口初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);    //APB2使能

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //shangla输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;     //           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void Send_ENC_Message(unsigned char user_code,unsigned char Cmd)
{
	//先发送引导码
	Send_IR(308);   //9ms发送载波信号 周期 = 9000us/26.3us = 342    -284
	NoSend_IR(158); //4.5ms不发送载波信号，周期 = 4500us/26.3us = 171  -158
	GetByte_And_SendByte(user_code,Cmd);   //取出每一位并发送
}

void GetByte_And_SendByte(unsigned char user_code,unsigned char Cmd)
{

		GetByte_SendByte(user_code);	//发送地址码
		GetByte_SendByte(~user_code);	//发送地址反码
		GetByte_SendByte(Cmd);			//发送命令码
		GetByte_SendByte(~Cmd);			//发送命令码的反码
		//发送结束码
		Send_NEC_0();
}
//取出数据中的每一位并发送
void GetByte_SendByte(unsigned char Cmd)
{	unsigned char temp,i; //定义中间变量
	temp=Cmd;
	for(i = 0;i<8;i++){  //循环8位命令中的每一位
		if(temp&0x01){  //如果是1执行
			Send_NEC_1();
					}
		else{     		//是0执行
			Send_NEC_0();
			}
		temp>>=1;
					  }
}
//-------------------------------------------------------------------------------------
/*
发送二进制数据 0
*/
void Send_NEC_0()
{
	Send_IR(20);  //发送载波信号0.56ms, 也就是发送红外光21个周期
	NoSend_IR(20);   //不发送载波信号0.56ms,也是个周期
}

/*
发送二进制数据 1
*/
void Send_NEC_1()
{
	Send_IR(20);    //发送载波信号也是21个周期 21*26.3=
	NoSend_IR(58);     //不发送载波信号为1.68ms 发送周期 = 1680us/26.3us = 63.87
}

//发送红外光， 26.3us这个周期内8.77us发送红外光，17.53us不发送红外光
void Send_IR(unsigned int i)
{
	while(i--)      //产生i个周期的信号 ，一个周期是26.3us
	{
		IR_EN1();
		CPU_TS_Tmr_Delay_US(8.77);//8.77us     
		IR_EN0();
		CPU_TS_Tmr_Delay_US(8.77*2);//8.77*2us   
	}

}

//不发送红外 26.3us这个周期内都不不发送红外光
void NoSend_IR(unsigned int i)
{
	while(i--)
	{
		IR_EN0();
		CPU_TS_Tmr_Delay_US(26.3);    
	}
}



