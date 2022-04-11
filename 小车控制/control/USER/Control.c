#include "UI.h"
#include "anbt_dmp_mpu6050.h"
extern uint16 ServoPwmDuty[8];
#define MIN_DIS_AVOID  70
u8 time=0;
//#define	LED PDout(2)  
u8 led_time=0;
int Motor_Speed[4];			//����ٶ�ֵ
int Motor_DrctAdj[4];		//���ת�����

unsigned char LX=127;
unsigned char LY=127;

PID PID_Motor[4];

u8 ARMED=0;
float Voltage_Count,Voltage_All;  //��ѹ������ر���
unsigned char nmr=0;

short int RC_Last=0;
unsigned char Contorl_stick=0;
unsigned char PS2_Stick=0;
unsigned char PS2_Flag=0;
unsigned char ServoCount=0;
unsigned char ULtrig=0;
unsigned int stage_now=1; //��ǰ�׶�
unsigned int f_noww=0; //���μ�ʱ
int angle_add=0;
int angle_last=0;
u32 key;

void Ps2_Control(void);
void Avoid_Control(void);
int angle_motor(int direction,int angle_d);//��
int motor_time(int x,int y,int z,int h,int time_c);//��
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//����ж�
	{		
		if(++ULtrig>=4)
		{
			ULtrig=0;
			Uln_Trig();	
		}
		//���ͳ����� new Ƶ�ʹ��� �ø�
		MPU6050_Pose();  
		Control_Flag=1;		
		if(++ServoCount>=4)
		{
			ServoCount=0;
			ServoPwmDutyCompare();
		}
		Voltage_All+=Get_Battery_vot();
		if(++Voltage_Count==100) Voltage=Voltage_All/100,Voltage_All=0,Voltage_Count=0;//��ƽ��ֵ ��ȡ��ص�ѹ	   
		if(KEY==0)	//��������
		{
			if(Keytimes<300)	Keytimes++;
		}
		else
		{
			if(Keytimes>30 && Keytimes<300)  {Kaysta=1;	N_Beep(1);}//��ǰ����̰�
			if(Keytimes==300)	{ Kaysta=2;N_Beep(2);}				//��ǰ�������
			Keytimes=0;
		}	
		if(PS2_Stick<8) PS2_Stick++;
		if(PS2_Stick==4)
		{	
			if(WorkMode==1){
				if(!PS2_RedLight()) //�ж��ֱ��Ƿ�Ϊ���ģʽ���ǣ�ָʾ��LED����
					PS2_Flag=1;	//���Ϊ���ģʽ
				else
					PS2_Flag=0; //���Ϊ�̵�ģʽ
			}
		}
		if(PS2_Stick==8)
		{	
			if(WorkMode==1){
				if(PS2_Flag)	//�Ǻ��ģʽ	
				{	
					key=PS2_DataKey();	 //�ֱ�����������
				}
			}
				PS2_Stick=0;
		}
		Contorl_stick++;
		if(Contorl_stick==4)
		{
			Contorl_stick=0;
			//Read_Encoder();//��������		
		}
	
		if(WorkMode==2)
		{
			switch(stage_now)
			{
				case 1:
					App_control_car();
					break;
				case 2:
					motor_time(-2000, -2000, -2000,-2000,1000);//ǰ��5���� ���Զ�stage++ ���� ���� ���� ����
					break;
				case 3:
					angle_motor(1,90);
					break;
				case 4:
					break;

				default:			
					break;
			}
			
		}
		if(WorkMode==1)
		{
			Ps2_Control();
		}
		if(WorkMode==3)
		{
			Auto_Traversing();
		}
		if(WorkMode==4)
		{
			Avoid_Control();
		}
	}	
	TIM6->SR&=~(1<<0);//����жϱ�־λ 	    
}

void Ps2_Control(void)
{	
		uint16 servoPwmDutyTemp;	
		PS2_CH[0]=	PS2_AnologData(PSS_RY);						
		PS2_CH[1]=	PS2_AnologData(PSS_RX);
		PS2_CH[2]=	PS2_AnologData(PSS_LX);
		Car_Control((PS2_CH[1]-127)*3/5,(127-PS2_CH[0])*3/5,(PS2_CH[2]-127)/2);   //get motor speed in x,y,yaw
	  Set_Motor(Motor_Speed[0]*25, Motor_Speed[1]*25,Motor_Speed[2]*25,Motor_Speed[3]*25); //then set motor speed
		switch (key)
		{
			case PSB_PAD_DOWN:
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
				break; 
			case PSB_PAD_UP:
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
				break;
			case PSB_PAD_RIGHT:
				servoPwmDutyTemp = ServoPwmDuty[0];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[0] = servoPwmDutyTemp;
				break; 
				
			case PSB_PAD_LEFT:
				servoPwmDutyTemp = ServoPwmDuty[0];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[0] = servoPwmDutyTemp;
				break;

			case PSB_TRIANGLE:
				servoPwmDutyTemp = ServoPwmDuty[2];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[2] = servoPwmDutyTemp;
				break;
			case PSB_CROSS:
				servoPwmDutyTemp = ServoPwmDuty[2];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[2] = servoPwmDutyTemp;
				break;
			case PSB_PINK:
				servoPwmDutyTemp = ServoPwmDuty[3];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[3] = servoPwmDutyTemp;
				break;
			case PSB_CIRCLE:
				servoPwmDutyTemp = ServoPwmDuty[3];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[3] = servoPwmDutyTemp;
				break;

			case PSB_L1:
				servoPwmDutyTemp = ServoPwmDuty[4];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[4] = servoPwmDutyTemp;
				break;
			case PSB_L2:
				servoPwmDutyTemp = ServoPwmDuty[4];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[4] = servoPwmDutyTemp;
				break;
			case PSB_R1:
				servoPwmDutyTemp = ServoPwmDuty[5];
				servoPwmDutyTemp += 2;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[5] = servoPwmDutyTemp;
				break;
			case PSB_R2:
				servoPwmDutyTemp = ServoPwmDuty[5];
				servoPwmDutyTemp -= 2;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[5] = servoPwmDutyTemp;
				break;
			default:break;
		}

}

void Line_Control(void)     //abandon
{
	 App_control_car();
	 Car_Control((127-LY)*3/5,(LX-127)*3/5,(127-127)/2);
}

void Avoid_Control(void)
{
	 if(Distance1>=MIN_DIS_AVOID)  //MIN=70
				//map(Distance1,MIN_DIS_AVOID,150,40,70);
				Set_Motor(-2000, -2000, -2000, -2000);          //ǰ�� �����  �Һ��� ��ǰ�� ��ǰ��
  else if(Distance1>50 && Distance1<MIN_DIS_AVOID){
				Set_Motor(-2000, 2000,2000, -2000);
  }
  else {
			Set_Motor(2000, 2000,2000, 2000);        //����
		
  }
}

void Car_Control(int Y_Move,int X_Move,int Yaw)  // distance control????
{
	Motor_Speed[0] =  (-Y_Move + X_Move + Yaw)*Motor_DrctAdj[0];	Motor_Speed[2] = (Y_Move - X_Move + Yaw)*Motor_DrctAdj[2];
	Motor_Speed[1] = 	(-Y_Move - X_Move + Yaw)*Motor_DrctAdj[1];	Motor_Speed[3] = (Y_Move + X_Move + Yaw)*Motor_DrctAdj[3];	
}

void PID_Init(void)
{	

	PID_Motor[0].Pdat=16.0;	PID_Motor[0].Idat=0.0;	PID_Motor[0].Ddat=4.0;	
	PID_Motor[1].Pdat=16.0;	PID_Motor[1].Idat=0.0;	PID_Motor[1].Ddat=4.0;
	PID_Motor[2].Pdat=16.0;	PID_Motor[2].Idat=0.0;	PID_Motor[2].Ddat=4.0;
	PID_Motor[3].Pdat=16.0;	PID_Motor[3].Idat=0.0;	PID_Motor[3].Ddat=4.0;
	Motor_DrctAdj[0]=1;Motor_DrctAdj[1]=1;Motor_DrctAdj[2]=1;Motor_DrctAdj[3]=1;


	PID_Motor[0].iout=0;
	PID_Motor[1].iout=0;
	PID_Motor[2].iout=0;
	PID_Motor[3].iout=0;
	
	PID_Motor[0].OUT=0;
	PID_Motor[1].OUT=0;
	PID_Motor[2].OUT=0;
	PID_Motor[3].OUT=0;
	
	PID_Motor[0].error=0;
	PID_Motor[1].error=0;
	PID_Motor[2].error=0;
	PID_Motor[3].error=0;
	
	PID_Motor[0].Last_error=0;
	PID_Motor[1].Last_error=0;
	PID_Motor[2].Last_error=0;
	PID_Motor[3].Last_error=0;

}

float Get_MxMi(float num,float max,float min)
{
	if(num>max)
		return max;
	else if(num<min)
		return min;
	else
		return num;
}

float map(float val,float I_Min,float I_Max,float O_Min,float O_Max)
{
	return (((val-I_Min)*((O_Max-O_Min)/(I_Max-I_Min)))+O_Min);
}

int motor_time(int x,int y,int z,int h,int time_c) //�趨���ٴΣ������Զ�stage++
{
	if(f_noww>=time_c)
	{
		stage_now++;
		f_noww=0;
		Set_Motor(0,0,0,0);
		return 0;
	}
	Set_Motor(x,y,z,h);
	f_noww++;
	return 0;
}
int angle_motor(int direction,int angle_d) //direction 0Ϊ�� 1Ϊ�� angle�������ٶ� ��ת�Ƕ�С��360�� ���� ���� ���� ����
{
	if(f_noww==0)
	{
		if(direction==1)
			Set_Motor(2000,-2000,2000,-2000);
		else
			Set_Motor(-2000,2000,-2000,2000);
		f_noww++;
	}
	else
	{
		if(direction==1)
		{
			if(Yaw <0 && angle_last>0)
			{
				angle_add+=(Yaw-angle_last)+360;
			}
			else
			angle_add+=(Yaw-angle_last);
		}
		else
		{
			if(Yaw >0 && angle_last<0)
			{
				angle_add+=(angle_last-Yaw)+360;
			}
			else
			angle_add+=(angle_last-Yaw);
		}

		if(angle_add>=angle_d)
		{
			Set_Motor(0,0,0,0);
			stage_now++;
			f_noww=0;
			angle_add=0;
			return 0;
		}
		f_noww++;
	}
	angle_last=Yaw;
	return 0;
}

