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

#ifndef _OLED_USER_H
#define _OLED_USER_H

#include "sys.h"
#include "oled.h"

#define DARK		0		//OLED的像素点不亮
#define LIGHT		1		//OLED的像素点亮

extern unsigned char OLED_Buffer[8][128];						//OLED显示缓存

void OLED_Draw_FreeLink(void);											//显示Logo
void OLED_Refresh_GDRAM(unsigned char displayxx);		//刷新显示
void OLED_Refresh_AllGDRAM(void);										//全屏刷显示

void OLED_Put12x24Char(u8 x, u8 y, u8 ch,u8 Col);		//显示12x24单个英文字符
void OLED_Put10x20Char(u8 x, u8 y, u8 ch,u8 Col);		//显示10x20单个英文字符
void OLED_Put8x16Char(u8 x, u8 y, u8 ch,u8 Col);		//显示8x16单个英文字符
void OLED_Put7x14Char(u8 x, u8 y, u8 ch,u8 Col);		//显示7x14单个英文字符
void OLED_Put6x12Char(u8 x, u8 y, u8 ch,u8 Col);		//显示6x12单个英文字符
void OLED_Put5x7Char(u8 x, u8 y, u8 ch,u8 Col);			//显示5x7单个英文字符
void OLED_Put24x24CNstr(u8 x,u8 y,u8 *Chstr,u8 Col);//显示多个24x24汉字 或 12x24英文字符
void OLED_Put20x20CNstr(u8 x,u8 y,u8 *Chstr,u8 Col);//显示多个20x20汉字 或 10x20英文字符
void OLED_Put16x16CNstr(u8 x,u8 y,u8 *Chstr,u8 Col);//显示多个16x16汉字 或 8x16英文字符
void OLED_Put14x14CNstr(u8 x,u8 y,u8 *Chstr,u8 Col);//显示多个14x14汉字 或 7x14英文字符
void OLED_Put12x12CNstr(u8 x,u8 y,u8 *Chstr,u8 Col);//显示多个12x12汉字 或 6x12英文字符
void OLED_Put5x7ENstr(u8 x,u8 y,u8 *ENstr,u8 Col);	//显示多个5x7英文字符

void OLED_DrawDot(u8 xx,u8 yy,u8 color);						//画点
void OLED_DrawLine_X(u8 X0,u8 X1,u8 Y,u8 Color);		//画横线
void OLED_DrawLine_Y(u8 X,u8 Y0,u8 Y1,u8 Color);		//画竖线
void OLED_DrawLine(short int X0, short int Y0, short int X1 ,short int Y1, short int Color);//画斜线
void OLED_DrawRectangle(u8 X0,u8 Y0,u8 X1,u8 Y1,u8 Color);		//画矩形
void OLED_DrawCircle(short int X0,short int Y0,short int R);	//画圆
void OLED_FillRectangle(u8 X0,u8 Y0,u8 X1,u8 Y1,u8 Color);		//矩形填充
void OLED_ClearBuffer(unsigned char Col);											//清除显示缓存

void OLED_Put6x12Num(u8 xx,u8 yy,int Num,u8 bitlen,u8 Col);		//显示6x12多位数字
void OLED_Put6x12Num1dot(u8 xx,u8 yy,int Num,u8 Col);					//显示6x12带1位小数点的数字
void OLED_Put5x7Num1dot(u8 xx,u8 yy,int Num);									//显示5x7带1位小数点的数字

#endif

/*************************************END************************************/



