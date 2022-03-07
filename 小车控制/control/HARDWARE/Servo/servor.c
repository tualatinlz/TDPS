#include "servor.h"
#include "usart.h"
#include "delay.h"
#define MAXPWM 2505		 //舵机最大PWM控制脉宽2.5ms宏定义
u8 count1;				 //每个变量用作8路舵机先后赋值控制
static bool Running = FALSE;	 //舵机运作标志位                       	
uint16 ServoPwmDuty[8] = {1500,1500,1500,1500,1500,1500,1500,1500};	//PWM脉冲宽度
uint16 ServoPwmDutySet[8] = {1500,1500,1500,1500,1500,1550,1500,1500};	//PWM脉冲宽度
float  ServoPwmDutyInc[8];		//为了速度控制，当PWM脉宽发生变化时，每2.5ms或20ms递增的PWM脉宽

bool ServoPwmDutyHaveChange = TRUE;	//脉宽有变化标志位

uint16 ServoTime = 2000;			//舵机从当前角度运动到指定角度的时间，也就是控制速度

void ServoSetPluseAndTime(unsigned int id,unsigned int p,unsigned int time)
{
	if((id > 0 || id ==0) && id <= 7 && p >= 500 && p <= 2500)
	{
		if(time < 20)
			time = 20;
		if(time > 30000)
			time = 30000;

		ServoPwmDutySet[id] = p;
		ServoTime = time;
		ServoPwmDutyHaveChange = TRUE;
		Running = TRUE;
	}
}


void ServoPwmDutyCompare(void)//脉宽变化比较及速度控制
{
	uint8 i;
	static bool ServoRunning = FALSE;	//舵机正在以指定速度运动到指定的脉宽对应的位置	
	static uint16 ServoPwmDutyIncTimes;	//需要递增的次数
	
	if(ServoPwmDutyHaveChange)//停止运动并且脉宽发生变化时才进行计算      ServoRunning == FALSE && 
	{
		ServoPwmDutyHaveChange = FALSE;
		ServoPwmDutyIncTimes = ServoTime/20;	//当每20ms调用一次ServoPwmDutyCompare()函数时用此句
		for(i=0;i<8;i++)
		{
			//if(ServoPwmDuty[i] != ServoPwmDutySet[i])
			{
				if(ServoPwmDutySet[i] > ServoPwmDuty[i])
				{
					ServoPwmDutyInc[i] = ServoPwmDutySet[i] - ServoPwmDuty[i];
					ServoPwmDutyInc[i] = -ServoPwmDutyInc[i];
				}
				else
				{
					ServoPwmDutyInc[i] = ServoPwmDuty[i] - ServoPwmDutySet[i];
					
				}
				ServoPwmDutyInc[i] /= ServoPwmDutyIncTimes;//每次递增的脉宽
			}
		}
		ServoRunning = TRUE;	//舵机开始动作
	}
	if(ServoRunning)
	{
		ServoPwmDutyIncTimes--;
		for(i=0;i<8;i++)
		{
			if(ServoPwmDutyIncTimes == 0)
			{		//最后一次递增就直接将设定值赋给当前值

				ServoPwmDuty[i] = ServoPwmDutySet[i];

				ServoRunning = FALSE;	//到达设定位置，舵机停止运动
				Running = FALSE;
			}
			else
			{

				ServoPwmDuty[i] = ServoPwmDutySet[i] + 
					(signed short int)(ServoPwmDutyInc[i] * ServoPwmDutyIncTimes);

			}
		}
		
	}
}

bool ServoAction(void)   //判断舵机是否在运作
{
	return Running;
}

void Servor_GPIO_Config(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable , ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_13|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
}
/************************GPIO电平反转函数1*****************************************/ 

void Flip_GPIO_One(void)
{
	switch(count1)								 //将20ms的舵机控制周期分成8份，每2.5ms控制一个舵机运转
	{  											      //每个定时器控制8路舵机运转，3个定时器控制24路舵机运转
		case 1: 
			 TIM1->ARR=ServoPwmDuty[0];				 //将第一个舵机脉冲宽度值赋值给定时器2			
    	 GPIO_SetBits(GPIOA,GPIO_Pin_13);  //同时拉高控制舵机1的引脚的电
				break;
		
		case 2:
   	    TIM1->ARR=MAXPWM-ServoPwmDuty[0]; 		 //将2.5ms减去PWM脉宽值的数据赋值定时器2
				GPIO_ResetBits(GPIOA,GPIO_Pin_13);//同时拉低控制舵机1引脚的电平 
				break;							 //控制舵机1的引脚在剩下20ms-CPM[0]时间内将一直保持低电平，舵机1按照CPWM值转动

		case 3:	
			TIM1->ARR=ServoPwmDuty[1]; 
				GPIO_SetBits(GPIOA,GPIO_Pin_11);		
				break;
		
		case 4:	
			TIM1->ARR=MAXPWM-ServoPwmDuty[1];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_11); 
				break;

		case 5:	
			TIM1->ARR=ServoPwmDuty[2];  
				GPIO_SetBits(GPIOA,GPIO_Pin_12); 
				break;
		
		case 6:	
			TIM1->ARR=MAXPWM-ServoPwmDuty[2];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_12);
				break;

		case 7:	
			 TIM1->ARR=ServoPwmDuty[3];  
			  GPIO_SetBits(GPIOD,GPIO_Pin_2); 
				break;
		
		case 8:	
			 TIM1->ARR=MAXPWM-ServoPwmDuty[3];  
				GPIO_ResetBits(GPIOD,GPIO_Pin_2);
				break;

		case 9:	
			TIM1->ARR=ServoPwmDuty[4];  
				GPIO_SetBits(GPIOA,GPIO_Pin_14); 
				break;
		
		case 10:
			TIM1->ARR=MAXPWM-ServoPwmDuty[4];  
				GPIO_ResetBits(GPIOA,GPIO_Pin_14);
				break;

		case 11:
			TIM1->ARR=ServoPwmDuty[5];  
				GPIO_SetBits(GPIOB,GPIO_Pin_14); 
				break;
		
		case 12:
			TIM1->ARR=MAXPWM-ServoPwmDuty[5];  
				GPIO_ResetBits(GPIOB,GPIO_Pin_14);
				break;

		case 13:
			TIM1->ARR=ServoPwmDuty[6];  
			//GPIO_SetBits(GPIOC,GPIO_Pin_6); 
				break;
		
		case 14:
			  TIM1->ARR=MAXPWM-ServoPwmDuty[6];  
			//	GPIO_ResetBits(GPIOC,GPIO_Pin_6);
				break;

		case 15:
			TIM1->ARR=ServoPwmDuty[7];  
     // GPIO_SetBits(GPIOC,GPIO_Pin_7); 
				break;
		
		case 16:
			TIM1->ARR=MAXPWM-ServoPwmDuty[7]; 
     // GPIO_ResetBits(GPIOC,GPIO_Pin_7);
			count1=0; 
				break;
		default:break;
	}	
}


/**********************************************************************************/   
/************************舵机控制函数1*********************************************/
void Servo1(void)
{		
	count1++; 
	Flip_GPIO_One();						 //反转IO电平

}

