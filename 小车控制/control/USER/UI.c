/***************************************************************************
������ɹ�ѧϰʹ�ã�����������Ҳ������������;��������������飬��ӭ���罻��ָ��
Ӳ���汾����·ֱ��������ư�
����оƬ��STM32F103RCT6
�������ƣ�STM32����ȫ���ƶ�С������--UI�������
����������UI�л����棬���������˻���������



*****************************************************************************/


#include "UI.h"
#include "anbt_dmp_mpu6050.h"
unsigned short int Keytimes=0;				//��������ʱ�����
unsigned char Kaysta=0;								//����״̬����
unsigned char WorkMode=0;							//����ģʽ����
unsigned int stage_ct=0;              // stage count 
unsigned int Voltage=0;									//��ص�ѹ��������λ0.1V
char Menux=0;													//�˵�����
unsigned char UI_Display_Flag=0;			//��ǽ���д��һ��
//short int	VoltLowWarn=0;							//�͵�ѹ����ֵ
//unsigned short int VoltLowTimes=0;		//��ѹ�ͼ�����
short int PS2_CH[3];
unsigned char Control_Flag=0;		
extern int output_mv,output_mv2;
unsigned char Title_Strings[5][13]={
"[Functions]\0",	"[ PS2 test ]\0",	"[Patio 1]\0","[Patio 2]\0","[Ultrasonic]\0",
};

unsigned char StandbyUI_String[3][6][7]={
"MV L: \0","MV R: \0","YAW:\0","DIS��\0","VOLT: \0","DIS: \0",
"FL�� \0","FR�� \0","BL��\0","BR��\0","vOLT: \0","dIS: \0",
"F_L�� \0","F_R�� \0","B_L�� \0","B_R��\0","Volt��\0","Dis��\0",
};
unsigned char StandbyUI_String2[2][21]={
"<shortswitch long Y>\0",
"<  short exit  >\0",
};

unsigned char SettingUI_String[4][11]={
"PS2 test \0","Patio 1 \0","Patio 2 \0","Ultrasonic\0",
};

u8 Standby_Mode(void)
{
	static u8 flag=1;
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		OLED_Put12x12CNstr(6,13,SettingUI_String[0],LIGHT);			OLED_Put12x12CNstr(75,13,SettingUI_String[1],LIGHT);
		OLED_Put12x12CNstr(6,26,SettingUI_String[2],LIGHT);			OLED_Put12x12CNstr(75,26,SettingUI_String[3],LIGHT);	
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[0],LIGHT);			//��ʾ��ʾ��
	
		UI_Display_Flag=0;
	}
	if(Kaysta==1)				//�����̰�
	{
	 	Menux++;	//ģʽ0��Standby
		if(Menux==5) Menux=1;
		UI_Display_Flag=1;	//��Ǹı����
		Kaysta=0;	//����������
										//����������
	}
	if(Kaysta==2)	//��������
	{
		switch(Menux)
		{
			case 1:WorkMode=1;break;
			case 2:WorkMode=2;break;
			case 3:WorkMode=3;break;
			case 4:WorkMode=4;break;
			//case 5:WorkMode=5;break;
			default:break;
	
		}
		UI_Display_Flag=1;						//��Ǹı����
		Kaysta=0;											//����������
		flag=0;
	}
	Display_Menux(Menux);	
	OLED_Refresh_AllGDRAM(); 	//ˢ����ʾ
	return flag;																				
}	

void Ps2_Mode(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);				
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//��ʾ��ʾ��
		ARMED=1;
		UI_Display_Flag=0;	
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
	OLED_Put6x12Num(32,13,output_mv,5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//��ʾת��
	OLED_Put5x7Num1dot(30,42,Voltage);		//��ʾ��ѹ	

}

void Blutooth_Mode(void)
{

	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//��ʾ��ʾ��
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
	OLED_Put6x12Num(32,13,output_mv,5,LIGHT);	OLED_Put6x12Num(98,13,output_mv2,5,LIGHT);		
	OLED_Put6x12Num(32,26,Yaw,5,LIGHT);	OLED_Put6x12Num(98,26,Distance1,5,LIGHT);	//��ʾת��
	OLED_Put5x7Num1dot(30,42,Voltage);		//��ʾ��ѹ
}

void Tracking_Mode(void)
{

	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//��ʾ��ʾ��
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}		
	OLED_Put6x12Num(32,13,output_mv,5,LIGHT);	OLED_Put6x12Num(98,13,output_mv2,5,LIGHT);		
	OLED_Put6x12Num(32,26,Yaw,5,LIGHT);	OLED_Put6x12Num(98,26,Distance1,5,LIGHT);	//��ʾת��
	OLED_Put5x7Num1dot(30,42,Voltage);		//��ʾ��ѹ	
}

void Avoid_Mode(void)
{
	//Uln_Trig();						//���ͳ�����
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//��ʾ��ʾ��
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
		OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//��ʾת��
		OLED_Put5x7Num1dot(30,42,Voltage);		//��ʾ��ѹ	
		OLED_Put6x12Num(98,42,Distance1,5,1);			//��ʾ��ص�ѹ
}
void Line_Mode(void)
{
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//��ʾ��ʾ��
		ARMED=1;
		UI_Display_Flag=0;
	}
	if(Kaysta==1)
	{
		WorkMode=0;	
		Menux=0;
		UI_Display_Flag=1;
		Kaysta=0;
	}
		OLED_Put6x12Num(32,13,Encoder[3],5,LIGHT);	OLED_Put6x12Num(98,13,Encoder[2],5,LIGHT);		
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//��ʾת��
		OLED_Put5x7Num1dot(30,42,Voltage);		//��ʾ��ѹ	
}



void Display_Menux(char Menuxx)
{
	if(Menuxx==1)	OLED_Put5x7Char(0,15,'>',LIGHT);
	else 					OLED_Put5x7Char(0,15,' ',LIGHT);
	if(Menuxx==2)	OLED_Put5x7Char(66,15,'>',LIGHT);
	else					OLED_Put5x7Char(66,15,' ',LIGHT);
	if(Menuxx==3)	OLED_Put5x7Char(0,28,'>',LIGHT);
	else					OLED_Put5x7Char(0,28,' ',LIGHT);
	if(Menuxx==4)	OLED_Put5x7Char(66,28,'>',LIGHT);
	else					OLED_Put5x7Char(0,41,' ',LIGHT);
	
}




	
