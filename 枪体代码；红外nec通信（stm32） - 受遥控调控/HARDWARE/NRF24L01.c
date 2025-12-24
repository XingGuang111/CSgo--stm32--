# include "NRF24L01.h"
#include "NRF24L01_Ins.h"
#include "stm32f10x.h"                  // Device header
#include "Delay.h"


#define TX_ADR_WIDTH    5     //5字节地址宽度
#define RX_ADR_WIDTH    5     //5字节地址宽度
#define TX_PLOAD_WIDTH  32    //32字节有效数据宽度
#define RX_PLOAD_WIDTH  32    //32字节有效数据宽度

const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 目的地址
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 本机地址


void W_SS(uint8_t BitValue)    //0或者1
{
	GPIO_WriteBit(CSN_Port, CSN_Pin, (BitAction)BitValue);// 对单个io至0或1
} 

void W_CE(uint8_t BitValue)
{
	GPIO_WriteBit(CE_Port, CE_Pin, (BitAction)BitValue);
} 

void W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(SCK_Port, SCK_Pin, (BitAction)BitValue);
} 

void W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(MOSI_Port, MOSI_Pin,(BitAction)BitValue);
} 

uint8_t R_MISO(void)
{
	return GPIO_ReadInputDataBit(MISO_Port, MISO_Pin);
	//读取指定GPIO管脚上的输入电平
}

uint8_t R_IRQ(void)
{
	return GPIO_ReadInputDataBit(IRQ_Port, IRQ_Pin);  
	//读取指定GPIO管脚上的输入电平
}

void NRF24L01_Pin_Init(void)	
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = CSN_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CSN_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = SCK_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SCK_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = MOSI_Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOSI_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = CE_Pin;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CE_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = MISO_Pin;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(MISO_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = IRQ_Pin;  			
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IRQ_Port, &GPIO_InitStructure);
	
}
		


uint8_t SPI_SwapByte(uint8_t Byte)  //通过SPI对24L01进行读写的函数 返回读取的字节
{
	uint8_t i;
	for(i = 0; i < 8; i ++) 
	{
		if((uint8_t)(Byte & 0x80) == 0x80)
		{
			W_MOSI(1);
		} 		
		else 
		{
			W_MOSI(0);
		}			
		Byte = (Byte << 1);			
		W_SCK(1);					
		Byte |= R_MISO();	        
		W_SCK(0);					
	}
	return Byte;
}
/*---------------------------------------------------------------------------*
NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value)		写NRF24L01寄存器

参数：Reg, 寄存器地址
	  Value：要写入的数据
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_Write_Reg(uint8_t Reg, uint8_t Value)
{
	uint8_t Status;

	W_SS(0);                  
  	Status = SPI_SwapByte(Reg);
	SPI_SwapByte(Value);	
	W_SS(1);                 

	return Status;
}
/*---------------------------------------------------------------------------*
NRF24L01_Read_Reg(uint8_t Reg)		读NRF24L01寄存器

参数：Reg：寄存器地址
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_Read_Reg(uint8_t Reg)
{
 	uint8_t Value;

	W_SS(0);              
  	SPI_SwapByte(Reg);		
	Value = SPI_SwapByte(NOP);
	W_SS(1);             	

	return Value;
}



/*---------------------------------------------------------------------------*
NRF24L01_Read_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)	一次读NRF24L01寄存器多个字节

参数：Reg：寄存器地址；*Buf：寄存器字节读出后存储的数组；Len：要读出的字节个数
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_Read_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
	uint8_t Status, i;
	W_SS(0);                    	
	Status =SPI_SwapByte(Reg);		
 	for(i = 0; i < Len; i ++)
	{
		Buf[i] = SPI_SwapByte(NOP);	
	}
	W_SS(1);                 		
	return Status;        			
}
/*---------------------------------------------------------------------------*
NRF24L01_Write_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)		一次写NRF24L01寄存器多个字节

参数：Reg：寄存器地址；*Buf：寄存器写入字节的存放数组；Len：要读出的字节个数
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_Write_Buf(uint8_t Reg, uint8_t *Buf, uint8_t Len)
{
	uint8_t Status, i;
	W_SS(0); 
	Status = SPI_SwapByte(Reg);		
	for(i = 0; i < Len; i ++)
	{
		SPI_SwapByte(*Buf ++); 		
	}
	W_SS(1); 
	return Status;          		
}							  					   
/*---------------------------------------------------------------------------*
NRF24L01_GetRxBuf(uint8_t *Buf)		读出接收到的数据，为多个字节

参数：*Buf多个字节存放的数组
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_GetRxBuf(uint8_t *Buf)
{
	uint8_t State;
	State = NRF24L01_Read_Reg(STATUS);  			
	NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State);
	if(State & RX_OK)								
	{                                                       
		W_CE(1);											
		NRF24L01_Read_Buf(RD_RX_PLOAD, Buf, RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX, NOP);					
		W_CE(1);				
		Delay_us(150);
		return 0; 
	}	   
	return 1;
}
/*---------------------------------------------------------------------------*
NRF24L01_SendTxBuf(uint8_t *Buf)	发送多个字节

参数：*Buf要发送的字节存放的数组
返回值：状态值
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_SendTxBuf(uint8_t *Buf)
{
	uint8_t State;
   
	W_CE(0);												
  	NRF24L01_Write_Buf(WR_TX_PLOAD, Buf, TX_PLOAD_WIDTH);	
 	W_CE(1);												
	while(R_IRQ() == 1);									
	State = NRF24L01_Read_Reg(STATUS);  					
	NRF24L01_Write_Reg(nRF_WRITE_REG + STATUS, State); 		
	if(State&MAX_TX)				 						
	{
		NRF24L01_Write_Reg(FLUSH_TX, NOP);					
		return MAX_TX; 
	}
	if(State & TX_OK)	
	{
		return TX_OK;
	}
	return NOP;	
}
/*---------------------------------------------------------------------------*
NRF24L01_Check(void)				检验NRF24L01是否存在

返回值：1：不存在，0：存在
*---------------------------------------------------------------------------*/

uint8_t NRF24L01_Check(void)
{
	uint8_t check_in_buf[5] = {0x11 ,0x22, 0x33, 0x44, 0x55};
	uint8_t check_out_buf[5] = {0x00};

	W_SCK(0);    //SCK=0  SPI时钟置低
	W_SS(1);     //CSN=1  SPI禁止
	W_CE(0);	  //CE=0    待机

	NRF24L01_Write_Buf(nRF_WRITE_REG + TX_ADDR, check_in_buf, 5);

	NRF24L01_Read_Buf(nRF_READ_REG + TX_ADDR, check_out_buf, 5);

	if((check_out_buf[0] == 0x11) && (check_out_buf[1] == 0x22) && (check_out_buf[2] == 0x33) && (check_out_buf[3] == 0x44) && (check_out_buf[4] == 0x55))
	{
		return 0;    //正确，退出
	}
	else 
	{
		return 1;    //重新读取
	}
}			

//NRF24L01初始化，
//包含检验NRF24L01是否存在、收发配置初始化，初始化直接调用该函数即可
void NRF24L01_RT_Init(void) 
{	
	W_CE(0);		  //CE=0    待机
  	NRF24L01_Write_Reg(nRF_WRITE_REG+RX_PW_P0, RX_PLOAD_WIDTH);
		NRF24L01_Write_Reg(FLUSH_RX, NOP);									
  	NRF24L01_Write_Buf(nRF_WRITE_REG + TX_ADDR, (uint8_t*)TX_ADDRESS, TX_ADR_WIDTH);
  	NRF24L01_Write_Buf(nRF_WRITE_REG + RX_ADDR_P0, (uint8_t*)RX_ADDRESS, RX_ADR_WIDTH);   
  	NRF24L01_Write_Reg(nRF_WRITE_REG + EN_AA, 0x01);     
  	NRF24L01_Write_Reg(nRF_WRITE_REG + EN_RXADDR, 0x01); 
  	NRF24L01_Write_Reg(nRF_WRITE_REG + SETUP_RETR, 0x1A);
  	NRF24L01_Write_Reg(nRF_WRITE_REG + RF_CH, 0);        
  	NRF24L01_Write_Reg(nRF_WRITE_REG + RF_SETUP, 0x0F);  
  	NRF24L01_Write_Reg(nRF_WRITE_REG + CONFIG, 0x0F);    
	W_CE(1);									  
}
//NRF24L01收发配置初始化
void NRF24L01_Init()
{
	NRF24L01_Pin_Init();
	while(NRF24L01_Check());
	NRF24L01_RT_Init();
	
}
/*---------------------------------------------------------------------------*
NRF24L01_SendBuf(uint8_t *Buf)		NRF24L01发送多个字节数据

参数：要发送字节存放的数组
*---------------------------------------------------------------------------*/
void NRF24L01_SendBuf(uint8_t *Buf)
{
	W_CE(0);									
	NRF24L01_Write_Reg(nRF_WRITE_REG + CONFIG, 0x0E);     //1.地址 2.内容
	W_CE(1);
	Delay_us(15);
	NRF24L01_SendTxBuf(Buf);						
	W_CE(0);
	NRF24L01_Write_Reg(nRF_WRITE_REG + CONFIG, 0x0F);
	W_CE(1);	
}
/*---------------------------------------------------------------------------*
uint8_t NRF24L01_Get_Value_Flag()	获取中断标志，一旦接收到数据，返回0

返回值：0：接收到数据；1：未接收到数据
*---------------------------------------------------------------------------*/
uint8_t NRF24L01_Get_Value_Flag()
{
	return R_IRQ();
}

