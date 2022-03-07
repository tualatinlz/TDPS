#include "exti.h"
#include "delay.h"

	   
//外部中断初始化程序
//初始化PA0/PE2/PE3/PE4为中断输入.
void EXTIX_Init(void)
{
	RCC->APB2ENR|=1<<3;    //使能PORTB时钟	   	 
	GPIOB->CRL&=0XFFFFFF00; 
	GPIOB->CRL|=0X00000088;//PB0,1 输入   	 
  GPIOB->ODR|=0x03;      //PB0,1 上拉 

	Ex_NVIC_Config(GPIO_B,0,FTIR); 			//下降沿触发
//	Ex_NVIC_Config(GPIO_B,1,FTIR); 			//下降沿触发

	MY_NVIC_Init(2,3,EXTI0_IRQn,2);	//抢占2，子优先级3，组2
//	MY_NVIC_Init(2,1,EXTI1_IRQn,2);	//抢占2，子优先级2，组2	   
// 	MY_NVIC_Init(2,1,EXTI3_IRQn,2);	//抢占2，子优先级1，组2	   
// 	MY_NVIC_Init(2,0,EXTI4_IRQn,2);	//抢占2，子优先级0，组2	   
}












