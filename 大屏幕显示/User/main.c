#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "NRF24L01.h"
/***************************************************************
以下为NRF24L01接发送数据的模板

发送的数据格式如下

[第一位，第二位，第三位，第四位.....，第三十三位]

其中，第一位也就是Buf[0]位接收到的数据长度
第二位到第三十三位一个32个字节的数据位接受到的数据；

例子如下，5代表后面跟着5个数据
{5, 0x11, 0x22, 0x33, 0x44, 0x55};

********************By:普普通通Ordinary*************************/
int falg1=0,falg2=0,falg3=0,falg4=0;
int  st[5];
int main(void)
{
	int  flag=2;
	uint8_t Buf[32] = {6, 0x11, 0x22, 0x33, 0x44, 0x55,0x66};
  uint8_t Buf_j[32] = {0};
	OLED_Init();
	NRF24L01_Init();
//	OLED_ShowString(1, 1, "Init Successful");
	NRF24L01_SendBuf(Buf);  //发送
	OLED_ShowString(1, 3,"id:");
	OLED_ShowString(1, 6,"xue:");
	OLED_ShowString(1, 10,"zd:");
	OLED_ShowString(1, 13,"zj:");
	while (1)
	{
		//接收
		if (NRF24L01_Get_Value_Flag() == 0)
		{
			NRF24L01_GetRxBuf(Buf_j);
		}
		//接收
		//显示
		if(Buf_j[1]==0x01)
		{
			if(falg1==0)
			{
						falg1=flag;
						flag++;
			}

		OLED_ShowNum(falg1, 1, Buf_j[0], 2);   //标志位
		OLED_ShowNum(falg1, 4, Buf_j[1], 2);   //id
		OLED_ShowNum(falg1, 7, Buf_j[3], 2);
		OLED_ShowNum(falg1, 10, Buf_j[4], 2);
		OLED_ShowNum(falg1, 13, Buf_j[5], 2);
		}
		else if(Buf_j[1]==0x02)
		{
					if(falg2==0)
			{
						falg2=flag;
						flag++;
				
			}
		OLED_ShowNum(falg2, 1, Buf_j[0], 2);   //标志位
		OLED_ShowNum(falg2, 4, Buf_j[1], 2);   //id
		OLED_ShowNum(falg2, 7, Buf_j[3], 2);
		OLED_ShowNum(falg2, 10, Buf_j[4], 2);
		OLED_ShowNum(falg2, 13, Buf_j[5], 2);
		}
		else if(Buf_j[1]==0x03)
		{
						if(falg3==0)
			{
						falg3=flag;
						flag++;
				
			}
		OLED_ShowNum(falg3, 1, Buf_j[0], 2);   //标志位
		OLED_ShowNum(falg3, 4, Buf_j[1], 2);   //id
		OLED_ShowNum(falg3, 7, Buf_j[3], 2);
		OLED_ShowNum(falg3, 10, Buf_j[4], 2);
		OLED_ShowNum(falg3, 13, Buf_j[5], 2);
		}
		else if(Buf_j[1]==0x04)
		{
					if(falg4==0)
			{
						falg4=flag;
						flag++;
				
			}
		OLED_ShowNum(falg4, 1, Buf_j[0], 2);   //标志位
		OLED_ShowNum(falg4, 4, Buf_j[1], 2);   //id
		OLED_ShowNum(falg4, 7, Buf_j[3], 2);
		OLED_ShowNum(falg4, 10, Buf_j[4], 2);
		OLED_ShowNum(falg4, 13, Buf_j[5], 2);
		}

		
	
	}
}
