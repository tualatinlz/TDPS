#include "delay.h"
static u8  fac_us=0;//us��ʱ������			   
static u16 fac_ms=0;//ms��ʱ������,

u32 a;
static __IO u32 TimingDelay;
/**************************�ж��������ú���*********************************************/
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
	if (SysTick_Config(SystemCoreClock/1000000))	// ST3.5.0��汾
	{ 
		while (1);
	}
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
#if 0
/***************************************************************************************/
/*************************΢����ʱ����**************************************************/
void Delay_us(u32 nTime)           
{ 
	TimingDelay = nTime;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/*************************������ʱ����**************************************************/
void Delay_ms(u32 nTime)           
{ 
	TimingDelay = nTime*1000;	
  
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while(TimingDelay != 0);
	
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}
/***************************************************************************************/
/**************************��ʱ���̴���*************************************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}

#endif
/***************************************************************************************/


void Delay_us(u32 nTime)
{    
   u16 i=0;  
   while(nTime--)
   {
      i=10;  
      while(i--) ;    
   }
}

void Delay_ms(u32 nTime)
{    
   u16 i=0;  
   while(nTime--)
   {
      i=12000; 
      while(i--) ;    
   }
}
			   
//��ʼ���ӳٺ���

//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(u8 SYSCLK)
{
 	SysTick->CTRL&=~(1<<2);	//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK/8;		//�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��
	fac_ms=(u16)fac_us*1000;//��ucos��,����ÿ��ms��Ҫ��systickʱ����   
}								    


//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //ʱ�����	  		 
	SysTick->VAL=0x00;        //��ռ�����
	SysTick->CTRL=0x01 ;      //��ʼ���� 	 
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void delay_ms(u16 nms)
{	 		  	  
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL =0x00;           //��ռ�����
	SysTick->CTRL=0x01 ;          //��ʼ����  
	do
	{
		temp=SysTick->CTRL;
	}
	while((temp&0x01)&&!(temp&(1<<16)));//�ȴ�ʱ�䵽��   
	SysTick->CTRL=0x00;       //�رռ�����
	SysTick->VAL =0X00;       //��ռ�����	  	    
} 		 
