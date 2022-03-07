/**********************************************************************************/
#include "timer.h"

/**********************定时器1中断配置函数*****************************************/
void TIM1_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}



/************************定时器配置函数************************************************/
/************************定时器配置函数************************************************/
void TIM1_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
    
	  TIM_DeInit(TIM1);											/* 重新启动定时器 */

    TIM_TimeBaseStructure.TIM_Period=2505;		 				/* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Prescaler=71;						/* 时钟预分频数 72M/72 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		/* 外部时钟采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 	/* 向上计数模式 */
  	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM1, TIM_FLAG_Update);						/* 清除溢出中断标志 */

    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);					/* 开启中断触发*/    
	
	  TIM_Cmd(TIM1,DISABLE);									    /* 关闭定时器 */ 
}

/**************************************************************************************/

/************************定时器6配置函数***********************************************/

//基本定时器6中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM6_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<4;//TIM6时钟使能    
 	TIM6->ARR=arr;  	//设定计数器自动重装值//刚好1ms    
	TIM6->PSC=psc;  	//预分频器7200,得到10Khz的计数时钟	  
	TIM6->DIER|=1<<0;   //允许更新中断	 
	TIM6->CR1|=0x01;    //使能定时器3
  MY_NVIC_Init(1,3,TIM6_IRQn,2);//抢占1，子优先级3，组2									 
}
void TIM7_Int_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<5;		//TIM7时钟使能    
 	TIM7->ARR=arr;  			//设定计数器自动重装值  
	TIM7->PSC=psc;  			//预分频器7200,得到10Khz的计数时钟	  
	TIM7->DIER&=~(1<<0);  //不允许更新中断	 
	TIM7->CR1&=~(0x01);   //不使能定时器3								 
}

/**************************************************************************************/
/************************定时器初始化函数*********************************************/
void Timer_Init(void)
{
	TIM1_NVIC_Configuration();
	TIM1_Configuration();
}
/**************************************************************************************/
/************************开启定时器函数************************************************/
void Timer_ON(void)
{	
	  TIM_Cmd(TIM1,ENABLE);	
}
/**************************************************************************************/
/************************关闭定时器函数************************************************/
void Timer_OFF(void)
{	
	
	TIM_Cmd(TIM1,DISABLE);
}
/**************************************************************************************/

