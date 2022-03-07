#include "sys.h"		
#include "delay.h"	
#include "exti.h"
#include "timer.h"	
#include "ULN.h"

unsigned int Distance1=0;

unsigned char TimeOut_Flag1=0;


void Uln_init(void)
{
	RCC->APB2ENR|=1<<4;    //使能PORTC时钟	   	 
	GPIOC->CRL&=0XFFFFFF0F; 
	GPIOC->CRL|=0X00000030;//PC1 输出  	 
	Trig=0;	
	TIM7_Int_Init(5000,720);//计数一个数值，时间为10us
	EXTIX_Init();
}


void Uln_Trig(void)
{
	Trig=1;Delay_us(100);Trig=0;
	if(TimeOut_Flag1)Distance1=500;
	TimeOut_Flag1=1;
	TIM7->CNT=0;  			//设定计数器自动重装值 
	TIM7->CR1|=0x01;    	//使能定时器3
}

void EXTI0_IRQHandler(void)
{
	Distance1=TIM7->CNT;//声速为340m/s,一个计数时间为10us,那么声音传播的距离为0.0034m,即0.34cm
	Distance1=Distance1*17/100-7; //单位0.1cm
	TimeOut_Flag1=0;
	EXTI->PR=1<<0;  //清除LINE1上的中断标志位  

}








