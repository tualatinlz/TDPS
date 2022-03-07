#include "led.h"
#include "delay.h"

 
//LED IO初始化
void LED_Init(void)
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能PORTB时钟
	

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PD2 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //设置为推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_SetBits(GPIOD,GPIO_Pin_2);				 //PD2 输出高


}
 
void Led_Test(void)
{
		LED1(ON);
	  Delay_ms(500);
	  LED1(OFF);
	  Delay_ms(500);
		LED1(ON);
	  Delay_ms(500);
		LED1(OFF);
	  Delay_ms(500);
	 
}
