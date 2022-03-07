/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--编码器程序
功能描述：四路AB相编码器的正交解码程序。电机转动时，可记录编码器脉冲数及电机转向。
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



#ifndef __ENCODER_H
#define __ENCODER_H
#include <sys.h>	 

extern short int Encoder[4];
extern short int Encoder_Sum[4];
void Encoder_Init(void);
void Encoder_Init_TIM8(void);
void Encoder_Init_TIM2(void);
void Encoder_Init_TIM3(void);
void Encoder_Init_TIM4(void);
void Read_Encoder(void);

#endif


/*************************************END************************************/

