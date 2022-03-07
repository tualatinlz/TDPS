/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--UI界面程序
功能描述：UI切换界面，调参数的人机交互界面


*****************************************************************************/


#ifndef _UI_H_
#define	_UI_H_

#include "sys.h"
#include "usart.h"
#include "led.h"
#include "delay.h"
#include "timer.h"
#include "ps2.h"
#include "beep.h"
#include "servor.h"
#include "motor.h"
#include "adc.h"
#include "key.h"
#include "oled.h"
#include "control_app.h"
#include "oled_user.h"
#include "Control.h"
#include "encoder.h"
#include "ULN.h"
#include "Traversing.h"
#include "servor.h"



extern unsigned short int Keytimes;					//按键按下时间计数
extern unsigned char Kaysta;								//按键状态变量
extern unsigned char WorkMode;							//工作模式变量
extern unsigned char Control_Flag;	
extern unsigned int Voltage;										//电池电压变量，单位0.1V
extern char Menux;													//菜单变量
extern unsigned short int AdjustAngle_Flag;	//角度校准标志位
extern short int Lang_x;										//语言种类
extern unsigned char UI_Display_Flag;				//标记界面写入一次
extern short int PS2_CH[3];
//extern short int	VoltLowWarn;							//低电压报警值
//extern unsigned short int VoltLowTimes;					//电压低检测次数
void Display_Menux(char Menuxx);
void Ps2_Mode(void);
void Blutooth_Mode(void);
void Avoid_Mode(void);
void Tracking_Mode(void);
u8 Standby_Mode(void);
void Line_Mode(void);

#endif


/*************************************END************************************/

