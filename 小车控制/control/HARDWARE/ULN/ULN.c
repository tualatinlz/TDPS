#include "sys.h"		
#include "delay.h"	
#include "exti.h"
#include "timer.h"	
#include "ULN.h"

float Distance1=0;  //��Ϊfloat
float Distance_last=0;

unsigned char TimeOut_Flag1=0;


void Uln_init(void)
{
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��	   	 
	GPIOC->CRL&=0XFFFFFF0F; 
	GPIOC->CRL|=0X00000030;//PC1 ���  	 
	Trig=0;	
	TIM7_Int_Init(5000,720);//����һ����ֵ��ʱ��Ϊ10us
	EXTIX_Init();
}


void Uln_Trig(void)
{
	Trig=1;Delay_us(100);Trig=0;
	if(TimeOut_Flag1)Distance1=400;
	TimeOut_Flag1=1;
	TIM7->CNT=0;  			//�趨�������Զ���װֵ 
	TIM7->CR1|=0x01;    	//ʹ�ܶ�ʱ��3
}

void EXTI0_IRQHandler(void)
{
	Distance_last=Distance1;
	Distance1=TIM7->CNT;//����Ϊ340m/s,һ������ʱ��Ϊ10us,��ô���������ľ���Ϊ0.0034m,��0.34cm
	Distance1=Distance1*17/100-20; //��λ0.1cm
	TimeOut_Flag1=0;
	EXTI->PR=1<<0;  //���LINE1�ϵ��жϱ�־λ  

}








