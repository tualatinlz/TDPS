/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--直流电机驱动程序
功能描述：PWM输出控制四路直流减速电机旋转，及旋转方向。
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


#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "sys.h"



#define MotorA1	PCout(5)
#define MotorA2	PCout(4)

#define MotorB1	PCout(3)
#define MotorB2	PCout(2)

#define MotorC1	PBout(5)
#define MotorC2	PBout(4)

#define MotorD1	PCout(8)
#define MotorD2	PCout(9)

void Motor_Init(void);//电机PWM初始化并设置电机的刷新速率50-499
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD);

#endif

/*************************************END************************************/



