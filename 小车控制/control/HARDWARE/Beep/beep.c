#include "beep.h"
#include "delay.h"


void Beep_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);  
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//PB12
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB12
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);				

}
 
void Beep_Test(void)
{
		BEEP(1);
	  Delay_ms(200);
		BEEP(0);
	  Delay_ms(200);
}

void N_Beep(char n)
{
	char i=0;
	for(i=0;i<n;i++)
	{
		BEEP(1);
	  Delay_ms(150);
		BEEP(0);
	  Delay_ms(150);
	}
}
