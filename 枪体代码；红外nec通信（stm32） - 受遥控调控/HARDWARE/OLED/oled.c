#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"
#include "delay.h"

//起始信号
void I2C_Start(void)
{
    OLED_SDIN_Set();
    OLED_SCLK_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

//结束信号
void I2C_Stop(void)
{
    OLED_SCLK_Set();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();
}

//等待信号响应
void I2C_WaitAck(void) //测数据信号的电平
{
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}

//写入一个字节
void Send_Byte(u8 dat)
{
    u8 i;
    for(i = 0; i < 8; i++)
    {
        OLED_SCLK_Clr();//将时钟信号设置为低电平
        if(dat & 0x80) //将dat的8位从最高位依次写入
        {
            OLED_SDIN_Set();
        }
        else
        {
            OLED_SDIN_Clr();
        }
        OLED_SCLK_Set();//将时钟信号设置为高电平
        OLED_SCLK_Clr();//将时钟信号设置为低电平
        dat <<= 1;
    }
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat, u8 mode)
{
    I2C_Start();
    Send_Byte(0x78);
    I2C_WaitAck();
    if(mode)
    {
        Send_Byte(0x40);
    }
    else
    {
        Send_Byte(0x00);
    }
    I2C_WaitAck();
    Send_Byte(dat);
    I2C_WaitAck();
    I2C_Stop();
}


uint8_t CMD_Data[] =
{
    0xAE, 0x00, 0x10, 0x40, 0xB0, 0x81, 0xFF, 0xA1, 0xA6, 0xA8, 0x3F,

    0xC8, 0xD3, 0x00, 0xD5, 0x80, 0xD8, 0x05, 0xD9, 0xF1, 0xDA, 0x12,

    0xD8, 0x30, 0x8D, 0x14, 0xAF
};      //初始化命令

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:15
* @brief: 写入初始化命令
* @return: None
********************************************************************************/
void Write_Cmd(void)
{
    int i;
    for (i = 0; i < 27; i++)
    {
        OLED_WR_Byte(CMD_Data[i], OLED_CMD); //SET DCDC命令
    }
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:19
* @brief: 向设备写控制命令
* @param: cmd
* @return: None
********************************************************************************/
void OLED_WR_CMD(uint8_t cmd)
{
    OLED_WR_Byte(cmd, OLED_CMD); //SET DCDC命令
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:35
* @brief: 写数据
* @param: data
* @return: None
********************************************************************************/
void OLED_WR_DATA(uint8_t data)
{
    OLED_WR_Byte(data, OLED_DATA); //SET DCDC命令
}


/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:32
* @brief: 清屏
* @param: None
* @return: None
********************************************************************************/
void OLED_Clear(void)
{
    uint8_t i, n;
    for (i = 0; i < 27; i++)
    {
        OLED_WR_CMD(0xb0 + i);
        OLED_WR_CMD(0x00);
        OLED_WR_CMD(0x10);
        for (n = 0; n < 128; n++)
        {
            OLED_WR_DATA(0);
        }
    }
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:59
* @brief: 开启OLed显示
* @param: None
* @return: None
********************************************************************************/
void OLED_Display_On(void)
{
    OLED_WR_CMD(0x8D);   //SET   DCDC设置电荷泵
    OLED_WR_CMD(0x14);   //DCDC  ON 开启电荷泵
    OLED_WR_CMD(0xAF);   //DISPlay ON开启显示
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 16:36
* @brief: 关闭OLED显示
* @param:
* @return:
********************************************************************************/
void OLED_Display_Off(void)
{
    OLED_WR_CMD(0x8D);   //SET   DCDC设置电荷泵
    OLED_WR_CMD(0x10);   //DCDC  OFF 开启电荷泵
    OLED_WR_CMD(0xAE);   //DISPlay OFF开启显示
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 16:33
* @brief: 设置起始点坐标
* @param: x,y
* @return: None
********************************************************************************/
void OLED_Set_Pos(uint8_t x, uint8_t y)
{
    OLED_WR_CMD(0xb0 + y);
    OLED_WR_CMD(((x & 0xf0) >> 4) | 0x10);
    OLED_WR_CMD(x & 0x0f);
}

void OLED_On(void)
{
    uint8_t i, n;
    for (i = 0; i < 8; i++)
    {
        OLED_WR_CMD(0xb0 + i);      //设置页地址
        OLED_WR_CMD(0x00);        //设置显示位置-列低地址
        OLED_WR_CMD(0x10);        //设置显示位置-列高地址
        for (n = 0; n < 128; n++)
        {
            OLED_WR_DATA(1);
        }//刷新显示
    }
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-03 15:35
* @brief: n为循环次数，得到值为m的n+1次方
* @param m
* @param n
* @return result
********************************************************************************/
unsigned int oled_pow(uint8_t m, uint8_t n)
{
    unsigned int result = 1;
    while (n--)result *= m;
    return result;
}

//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_ShowNum(uint8_t x, uint8_t y, unsigned int num, uint8_t len, uint8_t size2)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for (t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if (enshow == 0 && t < (len - 1))
        {
            if (temp == 0)
            {
                OLED_ShowChar(x + (size2 / 2) * t, y, ' ', size2);
                continue;
            }
            else enshow = 1;
        }
        OLED_ShowChar(x + (size2 / 2) * t, y, temp + '0', size2);
    }
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' ';//得到偏移后的值
    if (x > 128 - 1)
    {
        x = 0;
        y = y + 2;
    }
    if (Char_Size == 16)
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 8; i++)
            OLED_WR_DATA(F8X16[c * 16 + i]);
        OLED_Set_Pos(x, y + 1);
        for (i = 0; i < 8; i++)
            OLED_WR_DATA(F8X16[c * 16 + i + 8]);
    }
    else
    {
        OLED_Set_Pos(x, y);
        for (i = 0; i < 6; i++)
            OLED_WR_DATA(F6x8[c][i]);

    }
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-03 15:02
* @brief: 显示一个字符号串
* @param x
* @param y
* @param chr
* @param Char_Size
* @return: None
********************************************************************************/
void OLED_ShowString(uint8_t x, uint8_t y, uint8_t *chr, uint8_t Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        OLED_ShowChar(x, y, chr[j], Char_Size);
        x += 8;
        //如果--行显示的数字大于16就将行清零，y+2变成下一行
        if (x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-03 14:46
* @brief: 显示汉字     hzk 用取模软件得出的数组
* @param x,设置起始点x坐标
* @param y,设置起始点y坐标
* @param no,显示数组的第n个字
* @return:
********************************************************************************/
void OLED_ShowCHinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    OLED_Set_Pos(x, y);
    //显示上面一部分
    for (t = 0; t < 16; t++)
    {
        //Hzk[第几个数组][第几个值]
        OLED_WR_DATA(Hzk[2 * no][t]);
        adder += 1;
    }
    //显示下面一部分
    OLED_Set_Pos(x, y + 1);
    for (t = 0; t < 16; t++)
    {
        OLED_WR_DATA(Hzk[2 * no + 1][t]);
        adder += 1;
    }
}


/********************************************************************************
* @author: Luo Chen
* @date: 2022-07-01 15:38
* @brief: 初始化oled屏幕
* @param: None
* @return: None
********************************************************************************/
void OLED_Init(void)
{

    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(	OLED_CLK, ENABLE );

    GPIO_InitStructure.GPIO_Pin = OLED_SCLK_Pin | OLED_SDIN_Pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(OLED_PORT, &GPIO_InitStructure);

    Delay_ms(100);
    Write_Cmd();
    OLED_Clear();
}
void OLED_DrawBMP(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1)
{
	unsigned int j=0;
	uint8_t x,y;
	if(y1%8==0) y=y1/8;
	else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		for(x=x0;x<x1;x++)
		{
			OLED_WR_DATA(BMP[j++]);
		}
	}
}
