/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--OLED驱动程序
功能描述：驱动OLED显示字符、汉字、图像。
设计作者：水木蓝青@FreeLink
联系电话：15802781535
联系邮箱：357704597@qq.com
修改时间：2019/09/16
程序版本：V2.1
Copyright(C) 武汉飞令机器人科技有限公司 
All rights reserved
*****************************************************************************
修改说明：
无


*****************************************************************************/

#ifndef _OLED_H
#define _OLED_H

#include "sys.h"
#include "delay.h"



#define X_WIDTH 		128
#define Y_WIDTH 		64

//IO定义
//IO方向设置
#define OLED_SDA_IN()  {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=0X00000800;}
#define OLED_SDA_OUT() {GPIOC->CRH&=0XFFFFF0FF;GPIOC->CRH|=0X00000300;}

#define OLED_I2C_SCL        PCout(11) 
#define OLED_I2C_SDA        PCout(10)

#define READ_OLED_SDA      PCin(10)



//#define OLED_SCL(x)  PCout(11) = x
//#define OLED_SDA(x)  PCout(10) = x
//#define OLED_RST(x)  PCout(9) = x
//#define OLED_DC(x)   PCout(8) = x

void OLED_I2C_Init(void);
void OLED_I2C_Start(void);
void OLED_I2C_Stop(void);
u8 OLED_I2C_Wait_Ack(void);
void OLED_I2C_Send_Byte(u8 txd);

void OLED_Init(void);
void OLED_WriteDat(unsigned char dat);
void OLED_WriteCmd(unsigned char cmd);
void OLED_Set_Pos(unsigned char x,unsigned char y);
void OLED_Fill(unsigned char bmp_dat);



#endif

/*************************************END************************************/


