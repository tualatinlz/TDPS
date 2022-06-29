#include "UI.h"
#include "anbt_dmp_mpu6050.h"
extern uint16 ServoPwmDuty[8];
#define MIN_DIS_AVOID  70
u8 time=0;
//#define	LED PDout(2)  
u8 led_time=0;
char send_mode[4]={0}; //���л�ģʽ
int Motor_Speed[4];			//����ٶ�ֵ
int Motor_DrctAdj[4];		//���ת�����

unsigned char LX=127;
unsigned char LY=127;

PID PID_Motor[4];

u8 ARMED=1;   //�������������
float Voltage_Count,Voltage_All;  //��ѹ������ر���
unsigned char nmr=0;

short int RC_Last=0;
unsigned char Contorl_stick=0;
unsigned char PS2_Stick=0;
unsigned char PS2_Flag=0;
unsigned char ServoCount=0;
unsigned char ULtrig=0;
unsigned int stage_now=0; //��ǰ�׶�
unsigned int f_noww=0; //���μ�ʱ
int angle_add=0;
int angle_last=0;
u32 key;

void Ps2_Control(void);
void Avoid_Control(void);
int angle_motor(int direction,int angle_d);//��
int motor_time(int x,int y,int z,int h,int time_c);//��
int servor_time(int x,int y,int time_c);//��
int mode_change(int x);//�� �л�ģʽ
int motor_angle(int x,int y,int z,int h,float angle_mpu,int time_c); //�� �趨����Ƕ�ʱ�䣬�����Զ�stage++
void TIM6_IRQHandler(void)
{ 		    		  			    
	if(TIM6->SR&0X0001)//����ж�
	{		
		if(++ULtrig>=20)
		{
			ULtrig=0;
			//Uln_Trig();	
			Uln_Trig();	//���ͳ�����ȫ��
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
			Read_Encoder();//��������	
			if(ARMED)		
			{
				for(nmr=0;nmr<4;nmr++)
				{
					PID_Motor[nmr].error=Encoder[nmr]-Motor_Speed[nmr];
					//�����Pֵ
					PID_Motor[nmr].pout = PID_Motor[nmr].Pdat * PID_Motor[nmr].error;
					//�����Iֵ
					PID_Motor[nmr].iout += (PID_Motor[nmr].Idat*0.05) * PID_Motor[nmr].error;
					PID_Motor[nmr].iout = Get_MxMi(PID_Motor[nmr].iout,200,-200);//�ж�I�Ƿ񳬳���Χ
					//�����Dֵ
					PID_Motor[nmr].dout = PID_Motor[nmr].Ddat*0.05*(PID_Motor[nmr].error-PID_Motor[nmr].Last_error);	
					PID_Motor[nmr].Last_error=PID_Motor[nmr].error;	
					//PID����ֵ�ں�
					PID_Motor[nmr].OUT += PID_Motor[nmr].pout + PID_Motor[nmr].iout +PID_Motor[nmr].dout;////P,I,Dֵ���
					PID_Motor[nmr].OUT=Get_MxMi(PID_Motor[nmr].OUT,4599,-4599);
				}
				Set_Motor(PID_Motor[0].OUT, PID_Motor[1].OUT, PID_Motor[2].OUT, PID_Motor[3].OUT);
			 }
			else
			{
			//	Set_Motor(0, 0, 0, 0);
			}
		}
	
		if(WorkMode==2)
		{
			switch(stage_now)
			{
				case 0:
					stage_now++;
					break;
				case 1:
					mode_change(0x01);
					stage_now++;
					break;
				case 2:
				    
					App_control_car();
					//stage_now++; Ѳ�߲���
					if(Distance1<=25 && Distance_last<=25 && Distance1>0 && Distance_last>0)
					{	
						stage_now++;
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;

					}
						
					break;
				case 3:
					motor_time(-50, 50, 50,-50,32);  
					break;
				case 4:
					angle_motor(1,89); //��ǰ��ת
					break;
				case 5:
					motor_time(0, 0, 0,0,2000);
					
					break;
				case 6:
					stage_now++;
					break;
				
				case 7:
					motor_time(100, -100, -100,100,1330);
					//motor_time(100, -100, -100,100,1670);//ǰ������  ���Զ�stage++ //���� ����(��) ����(��) ���� 
					break;
				case 8:
					//App_control_car();
					angle_motor(0,93); //��ǰ�Ƕ���ת
					break;
				case 9:
					motor_time(0, 0, 0,0,2000);
					break;
				case 10:
					motor_time(100, -100, -100,100,1400);  //���ﹰ��ͣ��
					break;
				case 11:
					//ֹͣ
					break;
				case 12:
					
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
			//Auto_Traversing();
			switch(stage_now)
			{
				
				case 0:
					servor_time(1500,1350,400); //����趨 ������
					break;
				case 1:
					motor_time(50, -50, -50,50,1200); //ֱ�ߵ�ͼ��
					break;
				case 2:
					angle_motor(0,90); 
					break;
				case 3:
					stage_now++;
					break;
				case 4:
					motor_time(0, 0, 0,0,100); 
					break;
				case 5:
					mode_change(0x02);  //�л�ģʽ��
					App_control_car(); //��stage ʶ��ͼ��
					break;
				case 6:
					
					break;
				case 7:
					stage_now++;//����λ
					break;
				/************************************���*****************************************/
				case 8:
					motor_time(50, -50, -50,50,200); //ֱ�ߵ�ͼ��
					break;
				case 9:
					angle_motor(0,45); //����Ǹ����� ��
					break;
				case 10:
					motor_time(50, -50, -50,50,1600); //�Ƶ�
					break;
				case 11:
					angle_motor(1,45); //����
					break;
				case 12:
					motor_time(50, -50, -50,50,1100);
					break;
				case 13:
					angle_motor(1,80); //����
					break;
				case 14:
					Motor_Speed[0]=50; Motor_Speed[1]=-50; Motor_Speed[2]=-50; Motor_Speed[3]=50;
					if(Distance1<=30 && Distance_last<=30 && Distance1>0 && Distance_last>0)
					{
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;
						stage_now++;
						
					}
					break;
				case 15:
					motor_time(-50, 50, 50,-50,50); //ֱ�ߵ�ͼ��
					
					break;
				case 16:
					angle_motor(0,90); //��ת���� 
					break;
				
				

				case 17:
					stage_now=39; //ת����mv��
					break;
				/************************************�м�*****************************************/
				case 18:
					motor_time(50, -50, -50,50,100); //ֱ�ߵ�ͼ��
					break;
				case 19:
					stage_now++;
					break;
				case 20:
					motor_time(50, -50, -50,50,2300); //ֱ�Ƶ�
					break;
				case 21:
					angle_motor(1,90); //��ת��ȥλ��
					break;
				case 22:
					Motor_Speed[0]=50; Motor_Speed[1]=-50; Motor_Speed[2]=-50; Motor_Speed[3]=50;
					if(Distance1<=30 && Distance_last<=30 && Distance1>0 && Distance_last>0)
					{
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;
						stage_now++;
						
					}//ֱ�ߵ�����
					break;
				case 23:
					angle_motor(0,90); //������
					break;
				case 24:
					stage_now=39; //ת����mv
					break;
				/************************************�ұ�*****************************************/
				case 25:
					motor_time(50, -50, -50,50,200); //ֱ�ߵ�ͼ��
					break;
				case 26:
					angle_motor(1,45); //�����ұ�
					break;
				case 27:
					motor_time(50, -50, -50,50,1600); //ײ
					break;
				case 28:
					angle_motor(0,45); //�����
					break;
				case 29:
					motor_time(50, -50, -50,50,1100); //ײ
					break;
				case 30:
					stage_now++; //����
					break;
				
				case 31:
					stage_now++; //����
					break;
				case 32:
					stage_now++; //����
					break;
				case 33:
					stage_now++;//����λ
					break;
				case 34:
					stage_now++; //����
					break;
				case 35:
					stage_now++; //����
					break;
				case 36:
					stage_now++;
					break;
				case 37:
					stage_now++;
					break;
				case 38:
					stage_now=39; //ת����mv��
					break;
				case 39:
					delay_ms(3000);
					mode_change(0x03);  //�л�������ģʽ
					stage_now++;
					break;
				case 40:
					App_control_car(); //Ѳ��
					break;
				case 41:
					motor_time(40, -40, -40,40,50);
					break;
				case 42:
			     	angle_motor(1,90); //��ת
					break;
				case 43:
					motor_time(30, -30, -30,30,300); //ֱ��
					break;
				case 44:
					mode_change(0x03);  //�л�������ģʽ
					stage_now++;
					break;
				case 45:
					App_control_car();
					if(Distance1<=22 && Distance_last<=22 && Distance1>0 && Distance_last>0)
					{
						stage_now++;
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;
					}
					break;
				case 46:
					angle_motor(0,90);  //������	
					break;
				case 47:
					App_control_car(); //Ѳ��
					break;
				case 48:
					angle_motor(1,90); //��ת
					break;
				case 49:
					motor_time(40, -40, -40,40,400); //ֱ��
					break;
				case 50:
					mode_change(0x03);  //�л�������ģʽ
					stage_now++;
					break;
				case 51:
					App_control_car();  //Ѳ��
					if(Distance1<=23 && Distance_last<=23 && Distance1>0 && Distance_last>0)
					{
						stage_now++;
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;
					}
					break;
				case 52:
					angle_motor(0,90);  //��������ת
					break;
				case 53:
					mode_change(0x03);  //�л�������ģʽ
					stage_now++;
				case 54:
					
					App_control_car();  //Ѳ��
					if(Distance1<=23 && Distance_last<=23 && Distance1>0 && Distance_last>0)
					{
						stage_now++;
						Set_Motor(0, 0, 0, 0);
						Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0;
					}
					break;
				case 55:
					stage_now++;
					break;
				case 56:
					stage_now++;
					break;
				case 57:
					servor_time(1000,1350,1000); //����趨
					break;
				case 58:
					servor_time(1000,1000,1000); //����趨
					break;
				case 59:	
					servor_time(1500,1500,1000); //����趨�������
					break;
				case 60:
					angle_motor(0,90);  //��ת
					break;
				case 61:
					motor_time(100, -100, -100,100,4000); //�ߵ���������
					break;
				case 62:
					  	                        
					break;
				case 63:
					
					break;
				default:			
					break;
			}
		}
		if(WorkMode==4)
		{
			//Set_Motor(-1500,-1500,-1500,-1500); //���� ����(��) ����(��) ����
			servor_time(1000,1000,1000); //����趨
			//Avoid_Control();
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
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp += 10;
				if(servoPwmDutyTemp >= 2500)
				{
					servoPwmDutyTemp = 2500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
				break;
			case PSB_CROSS:
				servoPwmDutyTemp = ServoPwmDuty[1];
				servoPwmDutyTemp -= 10;
				if(servoPwmDutyTemp <= 500)
				{
					servoPwmDutyTemp = 500;
				}
				ServoPwmDuty[1] = servoPwmDutyTemp;
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
				Set_Motor(-2000, -2000, -2000, -2000);        //���� ����(��) ����(��) ����
  else if(Distance1>50 && Distance1<MIN_DIS_AVOID){
				Set_Motor(-2000, 2000,2000, -2000);
  }
  else {
			Set_Motor(2000, 2000,2000, 2000);        //����
		
  }
}

void Car_Control(int Y_Move,int X_Move,int Yaw)  // distance control????
{
	Motor_Speed[0] =  (-X_Move*1.1+-Yaw*2.5)*Motor_DrctAdj[0];	Motor_Speed[2] = ( X_Move*1.1+-Yaw*2.5)*Motor_DrctAdj[2];
	Motor_Speed[1] =  ( X_Move*1.1+-Yaw*2.5)*Motor_DrctAdj[1];	Motor_Speed[3] = (-X_Move*1.1+-Yaw*2.5)*Motor_DrctAdj[3];	
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

int servor_time(int x,int y,int time_c) //�趨һ�����������pwm���趨���ٴΣ������Զ�stage++
{
	if(f_noww>=time_c)
	{
		stage_now++;
		f_noww=0;
		return 0;
	}
	ServoPwmDuty[0]=x;
	ServoPwmDuty[1]=y;
	f_noww++;
	return 0;
}
int motor_angle(int x,int y,int z,int h,float angle_mpu,int time_c) //�趨����Ƕ�ʱ�䣬�����Զ�stage++ �ٶȲ������ù���
{
	float angle_error=Yaw-angle_mpu;
	if(f_noww>=time_c)
	{
		stage_now++;
		f_noww=0;
		Set_Motor(0,0,0,0);
		Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0; 
		return 0;
	}
	if(angle_error<=-180)
	{
		angle_error+=180;
	}
	else
	if(angle_error>=180)
	{
		angle_error-=180;
	} //angle_error����0Ӧ������ƫ
	Motor_Speed[0]=x+angle_error*5; Motor_Speed[1]=y-angle_error*5; Motor_Speed[2]=z-angle_error*5; Motor_Speed[3]=h+angle_error*5; //���� ����(��) ����(��) ���� 
	f_noww++;
	return 0;
}
int motor_time(int x,int y,int z,int h,int time_c) //�趨���ٴΣ������Զ�stage++
{
	if(f_noww>=time_c)
	{
		stage_now++;
		f_noww=0;
		Set_Motor(0,0,0,0);
		Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0; 
		return 0;
	}
	Motor_Speed[0]=x; Motor_Speed[1]=y; Motor_Speed[2]=z; Motor_Speed[3]=h; 
	f_noww++;
	return 0;
}
int angle_motor(int direction,int angle_d) //direction �Զ�stage++ 0Ϊ�� 1Ϊ�� 2Ϊ�ض��Ƕ� angle�������ٶ� ��ת�Ƕ�С��360�� //���� ����(��) ����(��) ���� �ͽ���˳���йأ�������
{
	if(f_noww==0)
	{
		if(direction==1)
		{
			angle_add=Yaw-angle_d;
			if(angle_add<-180)
				angle_add+=360;
		}
		else
		if(direction==0)
		{
			angle_add=Yaw+angle_d;
			if(angle_add>180)
				angle_add-=360;
		}
		else
		if(direction==2)
		{
			angle_add=angle_d;
		}
		f_noww++;
	}
	else
	{
		float error_Y;
		error_Y=angle_add-Yaw;
		if(error_Y<-180)
		{
			error_Y+=360;
		}
		else
		if(error_Y>180)
		{
			error_Y-=360;
		}
		if(error_Y<-1 )
		{
			Motor_Speed[0]=-1*error_Y; Motor_Speed[1]=-1*error_Y; Motor_Speed[2]=-1*error_Y; Motor_Speed[3]=-1*error_Y; //��ת
		}
		else
		if(error_Y>1)
		{
			Motor_Speed[0]=-1*error_Y; Motor_Speed[1]=-1*error_Y; Motor_Speed[2]=-1*error_Y; Motor_Speed[3]=-1*error_Y; 
		}

		if(abs(error_Y)<=1)
		{
			Motor_Speed[0]=0; Motor_Speed[1]=0; Motor_Speed[2]=0; Motor_Speed[3]=0; 
			Set_Motor(0,0,0,0);
			stage_now++;
			f_noww=0;
			angle_add=0;
			return 0;
		}
		f_noww++;
	}
	return 0;
}

int mode_change(int x)
{
	send_mode[0]=0xaa;
	send_mode[1]=0xff;
	send_mode[2]=x;
	send_mode[3]='\0';
	UART_PutStr(USART3,send_mode);
	memset(send_mode,0,sizeof(send_mode));
	return 0;
}
