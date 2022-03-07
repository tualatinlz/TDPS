#include "control_app.h"
#include "servor.h"
#include "timer.h"
#include "usart.h"
#include "beep.h"
#include "sys.h"
#include<string.h>
#include<stdlib.h>
#define TRUE   1
#define FALSE  0
extern unsigned char LX;
extern unsigned char LY;

extern unsigned char flag_RecFul;
extern char redata[500];   // 定义接收数据变量数组

uint16 UartRec[8]={1500,1500,1500,1500,1500,1500,1500,1500};

unsigned int pwm_num;

extern uint16 ServoPwmDuty[8];


int Is_Car_Front(const char *string)
{
	if(strncmp(string,"<BUPD>",6)==0 || strncmp(string,"BUPD",4)==0)
		return TRUE;
	else
		return FALSE;	
}

int Is_Car_Back(const char *string)
{
	if(strncmp(string,"<BDND>",6)==0 || strncmp(string,"BDND",4)==0)
		return TRUE;
	else
		return FALSE;	
}

int Is_Car_Left(const char *string)
{
	if(strncmp(string,"<BLTD>",6)==0||strncmp(string,"BLTD",4)==0)
		return TRUE;
	else
		return FALSE;	
}

int Is_Car_Right(const char *string)
{
	if(strncmp(string,"<BRTD>",6)==0 || strncmp(string,"BRTD",4)==0)	
		return TRUE;
	else
		return FALSE;
}

int Is_Car_Speed_Add(const char *string)
{
		if(strncmp(string,"BUAD",4)==0)	
		return TRUE;
	else
		return FALSE;
}

int Is_Car_Speed_Slow(const char *string)
{
		if(strncmp(string,"BUMD",4)==0)	
		return TRUE;
	else
		return FALSE;
}

int Is_Car_KD(const char *string)
{
		if(strncmp(string,"BUKD",4)==0)	
		return TRUE;
	else
		return FALSE;
}
int Is_Car_horning(const char *string)
{
		if(strncmp(string,"BUSD",4)==0)	
		return TRUE;
	else
		return FALSE;
}

int Is_Car_Nohorning(const char *string)
{
		if(strncmp(string,"BUSN",4)==0)	
		return TRUE;
	else
		return FALSE;
}

int Is_Car_Stop(const char *string)
{
	if((strncmp(string,"<BLTU>",6)==0) || (strncmp(string,"<BDNU>",6)==0) || \
		   (strncmp(string,"<BUPU>",6)==0)||(strncmp(string,"<BRTU>",6)==0) ||(strncmp(string,"BSTD",4)==0))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}




int Is_Duoji1_Angle(const char *string)
{
	if(strncmp(string,"<SUP-",5)==0 )
	{
		return TRUE;
	}
	else
		return FALSE;	
}

int Is_Duoji2_Angle(const char *string)
{
	if(strncmp(string,"<SLT-",5)==0 )
	{
		return TRUE;
	}
	else
		return FALSE;	
}

int Is_Duoji3_Angle(const char *string)
{
	if(strncmp(string,"<SDN-",5)==0)
	{
		return TRUE;
	}
	else
		return FALSE;	
}

int Is_Duoji4_Angle(const char *string)
{
	if(strncmp(string,"<SRT-",5)==0)
	{
		return TRUE;
	}
	else
		return FALSE;	
}

int Is_Duoji5_Angle(const char *string)
{
	if(strncmp(string,"<SDL-",5)==0)
	{
		return TRUE;
	}
	else
		return FALSE;	
}

int Is_Duoji6_Angle(const char *string)
{
	if(strncmp(string,"<SDR-",5)==0 )
	{
		return TRUE;
	}
	else
		return FALSE;	
}



int Is_LineControl(const char *string)
{
	if(strncmp(string,"<X",2)==0)
		return TRUE;
	else
		return FALSE;	
}

void LX_Control(const char *string)
{
	const char *pb = string;
	unsigned int LxValue;
	pb = strstr(string,"<X-");
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');
			pb++;
			LxValue = atol(pb);
	//	printf("angle1:%d",angle);
      LX = LxValue;
	}
}

void LY_Control(const char *string)
{
	const char *pb = string;
	unsigned int LyValue;
	pb = strstr(string,"Y-");
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');
			pb++;
			LyValue = atol(pb);
	//	printf("angle1:%d",angle);
      LY = LyValue;
	}
}

void DJ_angle_control_1(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SUP-");
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');
			pb++;
			angle = atol(pb);
	//	printf("angle1:%d",angle);
    ServoPwmDuty[1] = angle;
	}
}

void DJ_angle_control_2(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SLT-");
	
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');

			pb++;
			angle = atol(pb);	
		//printf("angle2:%d",angle);
		ServoPwmDuty[2] = angle;
   }

}

void DJ_angle_control_3(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SDN-");
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');
			pb++;
			angle = atol(pb);
		//printf("angle3:%d",angle);
    ServoPwmDuty[3] = angle;
	}

}

void DJ_angle_control_4(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SRT-");

	if(NULL!=pb)
	{
		pb = strchr(pb,'-');
			pb++;
			angle = atol(pb);
		//printf("angle4:%d",angle);
    ServoPwmDuty[4]=angle;
	}

}

void DJ_angle_control_5(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SDL-");

	if(NULL!=pb)
	{
		pb = strchr(pb,'-');	
			pb++;
			angle = atol(pb);

		//printf("angle5:%d",angle);
    ServoPwmDuty[5]=angle;
	}

}

void DJ_angle_control_6(const char *string)
{
	const char *pb = string;
	unsigned int angle;
	pb = strstr(string,"<SDR-");
	if(NULL!=pb )
	{
		pb = strchr(pb,'-');	
			pb++;
			angle = atol(pb); 
		//printf("angle6:%d",angle);
    ServoPwmDuty[6]=angle;
	}

}



#if 0
void OneLine(const char *str)
{
   char X_Postion=0;
   char Y_Postion=0;
   char x_now=0,y_now=0;
   char flag_x=0;
   char flag_y=0;
   unsigned char i=0;
   while(str[i]!='>')
	{
		if(flag_x==1)
		{
			if(str[i]!='Y')
			{  
	
			  x_now =str[i]-'0';	
			  X_Postion = (X_Postion*10+x_now);
			
			}
			else
			{
				flag_x=0;
			}
		}
		if(flag_y==1)
		{
			if(str[i]!='X')
			{
				y_now = str[i]-'0';	;
				Y_Postion=(Y_Postion*10+y_now);
			
			}
			else
			{
				flag_y=0;
			}
			if(str[i+1]=='>')
			{
				LX=X_Postion;
				LY=Y_Postion;
				i=0;
			}
		}
		if(str[i]=='X')
			flag_x=1;
		if(str[i]=='Y')
			flag_y=1;
		i++;
	
	}
}

#endif

void Servor_parse(const char *str)
{
	unsigned char motor_num=0;		   
	unsigned int  motor_jidu=0;	  
	unsigned int motor_time=0;	  
	unsigned char num_now=0;		   
	unsigned char jidu_now=0;		  
	unsigned char time_now=0;		   
	unsigned char flag_num=0;		  
	unsigned char flag_jidu=0;		  
	unsigned char flag_time=0;		   
	unsigned int i=0;

	while( str[i]!='\n'  && i < 500)
	{
		if(flag_num==1)	 				
		{
			if(str[i]!='P')				
			{
				num_now=str[i]-'0';
				motor_num=motor_num*10+num_now;
			}
			else  					
				flag_num=0;
		}

		if(flag_jidu==1)			
		{
			if((str[i]!='T')&(str[i]!='#'))	
			{							
				jidu_now=str[i]-'0';
				motor_jidu=motor_jidu*10+jidu_now;
			}
			else  						
			{
				flag_jidu=0;
				if(motor_jidu>2500)
					motor_jidu=2500;
			  UartRec[motor_num]=motor_jidu;
				pwm_num=motor_num;
				motor_jidu=0;
				motor_num=0;
			}
		}

		if(flag_time==1)				
		{
			time_now=str[i]-'0';
			motor_time=motor_time*10+time_now;
			UartRec[0]=motor_time;	   
			if(str[i+1]=='\r')
			{	
				if(motor_time<=100)		
				{
					ServoPwmDuty[0]=UartRec[1];
					ServoPwmDuty[1]=UartRec[2];
					ServoPwmDuty[2]=UartRec[3];
					ServoPwmDuty[3]=UartRec[4];
					ServoPwmDuty[4]=UartRec[5];
					ServoPwmDuty[5]=UartRec[6];
				}
				else
				{
					ServoSetPluseAndTime(0,UartRec[1],UartRec[0]);
					ServoSetPluseAndTime(1,UartRec[2],UartRec[0]);
					ServoSetPluseAndTime(2,UartRec[3],UartRec[0]);
					ServoSetPluseAndTime(3,UartRec[4],UartRec[0]);
					ServoSetPluseAndTime(4,UartRec[5],UartRec[0]);
					ServoSetPluseAndTime(5,UartRec[6],UartRec[0]);
				}
			}
		}
	
		if(str[i]=='#')
			flag_num=1;
		if(str[i]=='P')
			flag_jidu=1;
		if(str[i]=='T')
			flag_time=1;
		i++;
	}	  
}




void App_control(const char *str)  //UART control
{

	 if(Is_Car_Front(str))
		{

		}
	else if(Is_Car_Back(str))
		{	
			
		}
	else if(Is_Car_Left(str))
		{

		}
	else if(Is_Car_Right(str))
		{

		}
	else if(Is_Car_Stop(str))
		{

		}
  else if(Is_Duoji1_Angle(str))
		{
			DJ_angle_control_1(str);
		}
	else if(Is_Duoji2_Angle(str))
	 {
     DJ_angle_control_2(str);
   }
	 else if(Is_Duoji3_Angle(str))
	 {
     DJ_angle_control_3(str);
   }
	 else if(Is_Duoji4_Angle(str) )
	 {
     DJ_angle_control_4(str);
   }
   	 else if(Is_Duoji5_Angle(str))
	 {
     DJ_angle_control_5(str);
   }
   	 else if(Is_Duoji6_Angle(str))
	 {
     DJ_angle_control_6(str);
   }
	 else if(Is_Car_Speed_Add(str))
	 {

	 }
	 else if(Is_Car_Speed_Slow(str))
	 {

   }
	 else if(Is_LineControl(str))
	 {
			LX_Control(str);
		  LY_Control(str);
	 }
	 /*
	 else if(Is_Car_horning(str))
	 {
		 BUZZER=~BUZZER;
   }
	 else if(Is_Car_Nohorning(str))
	 {
		  BUZZER=1;
   }*/
	 else 
	 {
		Servor_parse(str);
   }
}

void App_control_car(void)
{
    if(flag_RecFul)
	{
	  flag_RecFul=0;
	  App_control(redata);
	}
}



