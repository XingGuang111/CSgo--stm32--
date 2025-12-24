#ifndef __HW_REMOTE_H
#define __HW_REMOTE_H

#include "sys.h"   
#define RDATA PBin(9)//红外数据输入引脚
#define REMOTE_ID 0   //选用遥控器识别码为0.

extern u8 RmtCnt; 
void Remote_Init(void); //功能引脚初始化
u8 Remote_Scan(void);     //定义功能函数
u8 re_add(void);
#endif
