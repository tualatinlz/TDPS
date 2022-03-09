#include "Traversing.h"
#include "motor.h"

void Traversing_GPIO_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1|GPIO_Pin_9|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void Auto_Traversing(void)
{
	if(D2==1 && D3 ==0 && D4==1 )  //直行 D2  PB15  D3 PB1 D4 PB9
		{
				Set_Motor(-1500, 1500,1500, -1500);
		}
	  else if (D2==0 && D3 ==1 && D4==1 ) 
		{
				Set_Motor(-1500, -1500,-1500, -1500);
		}
		else if ( D2==1 && D3 ==1 && D4==0) 
		{
				Set_Motor(1500, 1500,1500, 1500);	
	
		}


}

