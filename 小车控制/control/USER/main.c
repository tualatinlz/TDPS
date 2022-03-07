#include "UI.h"

u8 delay_50,delay_flag; 
			


int main(void)
{
	Stm32_Clock_Init(9);//ÏµÍ³Ê±ÖÓÉèÖÃ
	JTAG_Set(JTAG_SWD_DISABLE); //ÏÈ¹Ø±ÕJATG SWD¶Ë¿Ú¡£4Â·±àÂëÆ÷ÖÐ£¬ÐèÒªÕ¼ÓÃJTAG¶Ë¿ÚÖÐµÄ²¿·ÖÒý½Å
	AFIO->MAPR|=0x1<<8;					//timer2²¿·ÖÖØÓ³Éä£¬CH1---PA15,CH2---PB3£¨½âÂë±àÂëÆ÷µÄ4¸ötimerÖÐÓÐ¹²ÓÃµÄÒý½Å£¬Òò´ËÒªÖØÓ³Éä£
//	JTAG_Set(SWD_ENABLE); 			//Ö»¿ªÆôSWD¶Ë¿Ú¡££¨±ØÐë¿ªÆô£¬²»È»¶Ïµçºó£¬ÎÞ·¨µ÷ÊÔ»òÏÂÔØ³ÌÐò¡£´ËÊ±ÉèÖÃBOOT0Îª¸ßµçÆ½²Å¿ÉÒÔ£©
	//delay_init(72);			//ÑÓÊ±³õÊ¼»¯©
	Timer_Init();
	Timer_ON();
	//Uart_Init(1);	
	Uart_Init(3);	  //CONNECT TO PB10
	TIM6_Int_Init(5000,72);	  //5msÖÐ¶ÏÒç³ö   IN THE CONTROL.c
	//USART1_Config(115200);
	USART_Config(USART3,115200);
	//LED_Init();
//	PS2_Init();
	Beep_Init();
	KEY_Init();
	Servor_GPIO_Config();
	Motor_Init();
	//Encoder_Init();			//³õÊ¼»¯±àÂëÆ÷
	PID_Init();
	Set_Motor(0,0,0,0);
	Adc_Init();
	OLED_Init();
	Uln_init();								//³¬Éù²¨³õÊ¼»¯
	UI_Display_Flag=1;				//±ê¼Ç¸Ä±ä½çÃæ	
	PS2_CH[0]=0;PS2_CH[1]=0;PS2_CH[2]=0;

	while(Standby_Mode())	{	}	 //=====Ñ¡ÔñÔËÐÐÄ£Ê½ 
	if(WorkMode==1)	PS2_Init();
	else if(WorkMode==3) Traversing_GPIO_Init();
	while(1)
	{
		OLED_Refresh_AllGDRAM(); 	//Ë¢ÐÂÏÔÊ¾
		if(Control_Flag)
		{	
			switch(WorkMode)
			{
	////////////////////´ý»úÄ£Ê½////////////////////////
				case 0:
						Standby_Mode();
				break;
	////////////////////ÊÖ±úÄ£Ê½////////////////////////
				case 1:
					  Ps2_Mode();
					break;	
	////////////////////À¶ÑÀÄ£Ê½////////////////////////
				case 2:
						Blutooth_Mode();
					break;
	/////////////////////Ñ°¼£Ä£Ê½//////////////////////
				case 3:
						Tracking_Mode();
					break;
	///////////////////±ÜÕÏÄ£Ê½//////////////////////
				case 4:
						Avoid_Mode();			
					break;

				case 5:Line_Mode();
					break;

				case 6:break;

				case 7:break;						

				case 8:break;
				
				case 10:
							 break;

				case 11:
				case 12:
				case 13:
				case 14:
				break;	
				default:			break;
			}
			Control_Flag=0;
		}
	//	delay_flag=1;	
	//	delay_50=0;
	//	while(delay_flag);	     //Í¨¹ý¶¨Ê±ÖÐ¶ÏÊµÏÖµÄ50ms¾«×¼ÑÓÊ±	
	}
	
}
