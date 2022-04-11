/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾��FreeLink STM32��·ֱ��������ư�V2.41
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--ֱ�������������
����������PWM���������·ֱ�����ٵ����ת������ת����
*****************************************************************************
�޸�˵����
��


*****************************************************************************/


#include "motor.h"

void TIM5_PWM_Init(u16 arr,u16 psc)    
{

	RCC->APB1ENR|=1<<3;       //TIM5ʱ��ʹ��    
	RCC->APB2ENR|=1<<2;       //PORTAʱ��ʹ�� 
	
	GPIOA->CRL&=0XFFFF0000;   //PORTA0,1,2,3�������
	GPIOA->CRL|=0X0000BBBB;   //PORTA0,1,2,3�������
	
	TIM5->ARR=arr;//�趨�������Զ���װֵ 
	TIM5->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM5->CCMR1|=6<<4; //CH1 PWM1ģʽ	
	TIM5->CCMR1|=6<<12;//CH2 PWM1ģʽ	
	
	TIM5->CCMR2|=6<<4; //CH3 PWM1ģʽ
	TIM5->CCMR2|=6<<12;//CH4 PWM1ģʽ	

	TIM5->CCMR1|=1<<3; //CH1Ԥװ��ʹ��	
	TIM5->CCMR1|=1<<11;//CH2Ԥװ��ʹ��	
	TIM5->CCMR2|=1<<3; //CH3Ԥװ��ʹ��	
	TIM5->CCMR2|=1<<11;//CH4Ԥװ��ʹ��	 
	
	TIM5->CCER|=1<<0;  //CH1���ʹ��	
	TIM5->CCER|=1<<4;  //CH2���ʹ��	   
	TIM5->CCER|=1<<8;  //CH3���ʹ��	
	TIM5->CCER|=1<<12; //CH4���ʹ��	   
	
	TIM5->CR1=0x8000;  //ARPEʹ�� 
	TIM5->CR1|=0x01;   //ʹ�ܶ�ʱ��3 	

}
//�����ˢ������50-499
void Motor_Init(void)
{	
	TIM5_PWM_Init(4499,1); //3��Ƶ,PWMƵ��=72000000/(1+1)/(4499+1)=8khz
	TIM5->CCR1=0;				//����ռ�ձ�
	TIM5->CCR2=0;				//����ռ�ձ�
	TIM5->CCR3=0;				//����ռ�ձ�
	TIM5->CCR4=0;				//����ռ�ձ�
	
 	
	RCC->APB2ENR|=1<<4;       //GPIOCʱ��ʹ�� 
	GPIOC->CRL&=0XFF0000FF;   //PC2,3,4,5����Ϊǿ����
	GPIOC->CRL|=0X00333300;     
	GPIOC->ODR|=0xF<<2;      	//PC2,3,4,5����		

	RCC->APB2ENR|=1<<2;       //GPIOAʱ��ʹ��	
	GPIOC->CRH&=0XFFFFFF00;   //PC8,9����Ϊǿ����
	GPIOC->CRH|=0X00000033;  	   
	GPIOC->ODR|=0x3<<8;      	//PC8,9����		

	RCC->APB2ENR|=1<<3;       //GPIOBʱ��ʹ��	
	GPIOB->CRL&=0XFF00FFFF;   //PB4,5����Ϊǿ����
	GPIOB->CRL|=0X00330000;  	   
	GPIOB->ODR|=0x3<<4;      	//PB4,5����	
			
}
//���Ƶ��ת�� ǰ�� ���� ���� ���� ����  ���� ���� ���� ����
//1000-2000
void Set_Motor(int MotorA,int MotorB,int MotorC,int MotorD)  //
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

