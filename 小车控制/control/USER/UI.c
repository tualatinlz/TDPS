/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：四路直流电机控制板
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--UI界面程序
功能描述：UI切换界面，调参数的人机交互界面



*****************************************************************************/


#include "UI.h"

unsigned short int Keytimes=0;				//按键按下时间计数
unsigned char Kaysta=0;								//按键状态变量
unsigned char WorkMode=0;							//工作模式变量
unsigned int stage_ct=0;              // stage count 
unsigned int Voltage=0;									//电池电压变量，单位0.1V
char Menux=0;													//菜单变量
unsigned char UI_Display_Flag=0;			//标记界面写入一次
//short int	VoltLowWarn=0;							//低电压报警值
//unsigned short int VoltLowTimes=0;		//电压低检测次数
short int PS2_CH[3];
unsigned char Control_Flag=0;		

unsigned char Title_Strings[5][13]={
"[Functions]\0",	"[ PS2 test ]\0",	"[UART]\0","[trail]\0","[Ultrasonic]\0",
};

unsigned char StandbyUI_String[3][6][7]={
"F L: \0","F R: \0","B L:\0","B R：\0","VOLT: \0","DIS: \0",
"FL： \0","FR： \0","BL：\0","BR：\0","vOLT: \0","dIS: \0",
"F_L： \0","F_R： \0","B_L： \0","B_R：\0","Volt：\0","Dis：\0",
};
unsigned char StandbyUI_String2[2][21]={
"<shortswitch long Y>\0",
"<  short exit  >\0",
};

unsigned char SettingUI_String[4][11]={
"PS2 test \0","BlueTooth \0","trail \0","Ultrasonic\0",
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
		OLED_Put12x12CNstr(0,52,StandbyUI_String2[0],LIGHT);			//显示提示语
	
		UI_Display_Flag=0;
	}
	if(Kaysta==1)				//按键短按
	{
	 	Menux++;	//模式0，Standby
		if(Menux==5) Menux=1;
		UI_Display_Flag=1;	//标记改变界面
		Kaysta=0;	//清除按键标记
										//清除按键标记
	}
	if(Kaysta==2)	//按键长按
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
		UI_Display_Flag=1;						//标记改变界面
		Kaysta=0;											//清除按键标记
		flag=0;
	}
	Display_Menux(Menux);	
	OLED_Refresh_AllGDRAM(); 	//刷新显示
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
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
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
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	

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
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
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
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
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
		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
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
	OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
	OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
}

void Avoid_Mode(void)
{
	Uln_Trig();						//发送超声波
	if(UI_Display_Flag==1)
	{
		OLED_ClearBuffer(0);
		OLED_Put12x12CNstr(24,0,Title_Strings[WorkMode],LIGHT);
		
		OLED_Put12x12CNstr(0,13,StandbyUI_String[0][0],LIGHT);			OLED_Put12x12CNstr(66,13,StandbyUI_String[0][1],LIGHT);
		OLED_Put12x12CNstr(0,26,StandbyUI_String[0][2],LIGHT);			OLED_Put12x12CNstr(66,26,StandbyUI_String[0][3],LIGHT);
		OLED_Put12x12CNstr(0,39,StandbyUI_String[0][4],LIGHT);			OLED_Put12x12CNstr(66,39,StandbyUI_String[0][5],LIGHT);	

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
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
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
		OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
		OLED_Put6x12Num(98,42,Distance1,5,1);			//显示电池电压
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

		OLED_Put12x12CNstr(28,52,StandbyUI_String2[1],LIGHT);			//显示提示语
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
		OLED_Put6x12Num(32,26,Encoder[0],5,LIGHT);	OLED_Put6x12Num(98,26,Encoder[1],5,LIGHT);	//显示转速
		OLED_Put5x7Num1dot(30,42,Voltage);		//显示电压	
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




	
