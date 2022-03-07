/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--独立按键程序
功能描述：独立按键程序

*****************************************************************************
修改说明：
无


*****************************************************************************/


#include "key.h"
#include "delay.h"
						    
//按键初始化函数
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
} 


/*************************************END************************************/
