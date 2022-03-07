/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--直流电机驱动程序
功能描述：PWM输出控制四路直流减速电机旋转，及旋转方向。
*****************************************************************************
修改说明：
无


*****************************************************************************/


#include "motor.h"

void TIM5_PWM_Init(u16 arr,u16 psc)    
{

	RCC->APB1ENR|=1<<3;       //TIM5时钟使能    
	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
	
	GPIOA->CRL&=0XFFFF0000;   //PORTA0,1,2,3复用输出
	GPIOA->CRL|=0X0000BBBB;   //PORTA0,1,2,3复用输出
	
	TIM5->ARR=arr;//设定计数器自动重装值 
	TIM5->PSC=psc;//预分频器不分频
	
	TIM5->CCMR1|=6<<4; //CH1 PWM1模式	
	TIM5->CCMR1|=6<<12;//CH2 PWM1模式	
	
	TIM5->CCMR2|=6<<4; //CH3 PWM1模式
	TIM5->CCMR2|=6<<12;//CH4 PWM1模式	

	TIM5->CCMR1|=1<<3; //CH1预装载使能	
	TIM5->CCMR1|=1<<11;//CH2预装载使能	
	TIM5->CCMR2|=1<<3; //CH3预装载使能	
	TIM5->CCMR2|=1<<11;//CH4预装载使能	 
	
	TIM5->CCER|=1<<0;  //CH1输出使能	
	TIM5->CCER|=1<<4;  //CH2输出使能	   
	TIM5->CCER|=1<<8;  //CH3输出使能	
	TIM5->CCER|=1<<12; //CH4输出使能	   
	
	TIM5->CR1=0x8000;  //ARPE使能 
	TIM5->CR1|=0x01;   //使能定时器3 	

}
//电机的刷新速率50-499
void Motor_Init(void)
{	
	TIM5_PWM_Init(4499,1); //3分频,PWM频率=72000000/(1+1)/(4499+1)=8khz
	TIM5->CCR1=0;				//设置占空比
	TIM5->CCR2=0;				//设置占空比
	TIM5->CCR3=0;				//设置占空比
	TIM5->CCR4=0;				//设置占空比
	
 	
	RCC->APB2ENR|=1<<4;       //GPIOC时钟使能 
	GPIOC->CRL&=0XFF0000FF;   //PC2,3,4,5设置为强推挽
	GPIOC->CRL|=0X00333300;     
	GPIOC->ODR|=0xF<<2;      	//PC2,3,4,5上拉		

	RCC->APB2ENR|=1<<2;       //GPIOA时钟使能	
	GPIOC->CRH&=0XFFFFFF00;   //PC8,9设置为强推挽
	GPIOC->CRH|=0X00000033;  	   
	GPIOC->ODR|=0x3<<8;      	//PC8,9上拉		

	RCC->APB2ENR|=1<<3;       //GPIOB时钟使能	
	GPIOB->CRL&=0XFF00FFFF;   //PB4,5设置为强推挽
	GPIOB->CRL|=0X00330000;  	   
	GPIOB->ODR|=0x3<<4;      	//PB4,5上拉	
			
}
//控制电机转速
//1000-2000
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD)
{
	
	if(MotorA>0)
	{
		MotorA1=0,			MotorA2=1;
		TIM5->CCR2=MotorA;
	}
	else
	{
		MotorA1=1,			MotorA2=0;
		TIM5->CCR2=-MotorA;
	}
	
/////////////////////////////	
	if(MotorB>0)
	{
		MotorB1=0,			MotorB2=1;
		TIM5->CCR1=MotorB;
	}
	else
	{
		MotorB1=1,			MotorB2=0;
		TIM5->CCR1=-MotorB;
	}
///////////////////////////////////////	
	
	if(MotorC>0)
	{
		MotorC1=0,			MotorC2=1;
		TIM5->CCR4=MotorC;
	}
	else
	{
		MotorC1=1,			MotorC2=0;
		TIM5->CCR4=-MotorC;
	}
/////////////////////////////	
	if(MotorD>0)
	{
		MotorD1=0,			MotorD2=1;
		TIM5->CCR3=MotorD;
	}
	else
	{
		MotorD1=1,			MotorD2=0;
		TIM5->CCR3=-MotorD;
	}
}


/*************************************END************************************/

