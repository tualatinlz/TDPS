#include "UI.h"
#include "anbt_dmp_mpu6050.h"
u8 delay_50,delay_flag; 
			


int main(void)
{
	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);
	JTAG_Set(JTAG_SWD_DISABLE); //�ȹر�JATG SWD�˿ڡ�4·�������У���Ҫռ��JTAG�˿��еĲ�������
	AFIO->MAPR|=0x1<<8;					//timer2������ӳ�䣬CH1---PA15,CH2---PB3�������������4��timer���й��õ����ţ����Ҫ��ӳ��?
    //JTAG_Set(SWD_ENABLE); 			//ֻ����SWD�˿ڡ������뿪������Ȼ�ϵ���޷����Ի����س��򡣴�ʱ����BOOT0Ϊ�ߵ�ƽ�ſ��ԣ�
	//delay_init(72);			//��ʱ��ʼ��?
	Timer_Init();
	Timer_ON();
	OLED_Init();
	//Uart_Init(1);	
	Uart_Init(3);	  //CONNECT TO PB10
	TIM6_Int_Init(5000,72);	  //5ms�ж����   IN THE CONTROL.c 
	//USART1_Config(115200);
	USART_Config(USART3,115200);
	//LED_Init();
    //PS2_Init();
	Beep_Init();
	KEY_Init();
	Servor_GPIO_Config();
	Motor_Init();
	Encoder_Init();			//��ʼ��������
	PID_Init();
	Set_Motor(0,0,0,0); 
	Adc_Init();
	Uln_init();								//��������ʼ��
	delay_ms(250);  			//�����ӳ� ��ȻI2C������
	AnBT_DMP_MPU6050_Init();	//6050DMP��ʼ��,��У׼	
	UI_Display_Flag=1;				//��Ǹı����	
	PS2_CH[0]=0;PS2_CH[1]=0;PS2_CH[2]=0;
	mode_change(0xff);

	while(Standby_Mode())	{	}	 //=====ѡ������ģʽ 
	if(WorkMode==1)	PS2_Init();
	else if(WorkMode==3) Traversing_GPIO_Init();
	while(1)
	{
		OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
		if(Control_Flag) //Ƶ��ͬ control�ж�
		{	
			
			switch(WorkMode)
			{
	////////////////////����ģʽ////////////////////////
				case 0:
						Standby_Mode();
				break;
	////////////////////�ֱ�ģʽ////////////////////////
				case 1:
					  	Ps2_Mode();
					break;	
	////////////////////UARTģʽ////////////////////////
				case 2:
						Blutooth_Mode();
					break;
	/////////////////////Ѱ��ģʽ//////////////////////
				case 3:
						Tracking_Mode();
					break;
	///////////////////����ģʽ//////////////////////
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
	//	while(delay_flag);	     //ͨ����ʱ�ж�ʵ�ֵ�50ms��׼��ʱ	
	}
	
}
