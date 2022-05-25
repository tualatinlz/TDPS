#include "servor.h"
#include "usart.h"
#include "delay.h"
#define MAXPWM 2505		 //������PWM��������2.5ms�궨��
u8 count1;				 //ÿ����������8·����Ⱥ�ֵ����
static bool Running = FALSE;	 //���������־λ                       	
uint16 ServoPwmDuty[8] = {1500,1000,1500,1500,1500,1500,1500,1500};	//PWM������
uint16 ServoPwmDutySet[8] = {1500,1000,1500,1500,1500,1550,1500,1500};	//PWM������
float  ServoPwmDutyInc[8];		//Ϊ���ٶȿ��ƣ���PWM�������仯ʱ��ÿ2.5ms��20ms������PWM����

bool ServoPwmDutyHaveChange = TRUE;	//�����б仯��־λ

uint16 ServoTime = 2000;			//����ӵ�ǰ�Ƕ��˶���ָ���Ƕȵ�ʱ�䣬Ҳ���ǿ����ٶ�

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


void ServoPwmDutyCompare(void)//����仯�Ƚϼ��ٶȿ���
{
	uint8 i;
	static bool ServoRunning = FALSE;	//���������ָ���ٶ��˶���ָ���������Ӧ��λ��	
	static uint16 ServoPwmDutyIncTimes;	//��Ҫ�����Ĵ���
	
	if(ServoPwmDutyHaveChange)//ֹͣ�˶������������仯ʱ�Ž��м���      ServoRunning == FALSE && 
	{
		ServoPwmDutyHaveChange = FALSE;
		ServoPwmDutyIncTimes = ServoTime/20;	//��ÿ20ms����һ��ServoPwmDutyCompare()����ʱ�ô˾�
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
				ServoPwmDutyInc[i] /= ServoPwmDutyIncTimes;//ÿ�ε���������
			}
		}
		ServoRunning = TRUE;	//�����ʼ����
	}
	if(ServoRunning)
	{
		ServoPwmDutyIncTimes--;
		for(i=0;i<8;i++)
		{
			if(ServoPwmDutyIncTimes == 0)
			{		//���һ�ε�����ֱ�ӽ��趨ֵ������ǰֵ

				ServoPwmDuty[i] = ServoPwmDutySet[i];

				ServoRunning = FALSE;	//�����趨λ�ã����ֹͣ�˶�
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

bool ServoAction(void)   //�ж϶���Ƿ�������
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
/************************GPIO��ƽ��ת����1*****************************************/ 

void Flip_GPIO_One(void)
{
	switch(count1)								 //��20ms�Ķ���������ڷֳ�8�ݣ�ÿ2.5ms����һ�������ת
	{  											      //ÿ����ʱ������8·�����ת��3����ʱ������24·�����ת
		case 1: 
			 TIM1->ARR=ServoPwmDuty[0];				 //����һ�����������ֵ��ֵ����ʱ��2			
    	 GPIO_SetBits(GPIOA,GPIO_Pin_13);  //ͬʱ���߿��ƶ��1�����ŵĵ�
				break;
		
		case 2:
   	    TIM1->ARR=MAXPWM-ServoPwmDuty[0]; 		 //��2.5ms��ȥPWM����ֵ�����ݸ�ֵ��ʱ��2
				GPIO_ResetBits(GPIOA,GPIO_Pin_13);//ͬʱ���Ϳ��ƶ��1���ŵĵ�ƽ 
				break;							 //���ƶ��1��������ʣ��20ms-CPM[0]ʱ���ڽ�һֱ���ֵ͵�ƽ�����1����CPWMֵת��

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
/************************������ƺ���1*********************************************/
void Servo1(void)
{		
	count1++; 
	Flip_GPIO_One();						 //��תIO��ƽ

}

