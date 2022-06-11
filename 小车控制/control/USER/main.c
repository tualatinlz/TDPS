#include "UI.h"
#include "anbt_dmp_mpu6050.h"
u8 delay_50,delay_flag; 
			


int main(void)
{
	Stm32_Clock_Init(9);//系统时钟设置
	delay_init(72);
	JTAG_Set(JTAG_SWD_DISABLE); //先关闭JATG SWD端口。4路编码器中，需要占用JTAG端口中的部分引脚
	AFIO->MAPR|=0x1<<8;					//timer2部分重映射，CH1---PA15,CH2---PB3（解码编码器的4个timer中有共用的引脚，因此要重映射?
    //JTAG_Set(SWD_ENABLE); 			//只开启SWD端口。（必须开启，不然断电后，无法调试或下载程序。此时设置BOOT0为高电平才可以）
	//delay_init(72);			//延时初始化?
	Timer_Init();
	Timer_ON();
	OLED_Init();
	//Uart_Init(1);	
	Uart_Init(3);	  //CONNECT TO PB10
	TIM6_Int_Init(5000,72);	  //5ms中断溢出   IN THE CONTROL.c 
	//USART1_Config(115200);
	USART_Config(USART3,115200);
	//LED_Init();
    //PS2_Init();
	Beep_Init();
	KEY_Init();
	Servor_GPIO_Config();
	Motor_Init();
	Encoder_Init();			//初始化编码器
	PID_Init();
	Set_Motor(0,0,0,0); 
	Adc_Init();
	Uln_init();								//超声波初始化
	delay_ms(250);  			//必须延迟 不然I2C有问题
	AnBT_DMP_MPU6050_Init();	//6050DMP初始化,不校准	
	UI_Display_Flag=1;				//标记改变界面	
	PS2_CH[0]=0;PS2_CH[1]=0;PS2_CH[2]=0;
	mode_change(0xff);

	while(Standby_Mode())	{	}	 //=====选择运行模式 
	if(WorkMode==1)	PS2_Init();
	else if(WorkMode==3) Traversing_GPIO_Init();
	while(1)
	{
		OLED_Refresh_AllGDRAM(); 	//刷新显示
		if(Control_Flag) //频率同 control中断
		{	
			
			switch(WorkMode)
			{
	////////////////////待机模式////////////////////////
				case 0:
						Standby_Mode();
				break;
	////////////////////手柄模式////////////////////////
				case 1:
					  	Ps2_Mode();
					break;	
	////////////////////UART模式////////////////////////
				case 2:
						Blutooth_Mode();
					break;
	/////////////////////寻迹模式//////////////////////
				case 3:
						Tracking_Mode();
					break;
	///////////////////避障模式//////////////////////
				case 4:
						Avoid_Mode();			
					break;

				case 5:Line_Mode();
					break;

				case 6:
				break;

				case 7:
				break;						

				case 8:
				break;
				
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
	//	while(delay_flag);	     //通过定时中断实现的50ms精准延时	
	}
	
}
