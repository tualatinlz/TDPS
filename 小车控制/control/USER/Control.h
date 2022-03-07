#ifndef __CONTROL_H__
#define __CONTROL_H__

#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h" 
#include "beep.h"	 	 
#include "key.h"	 	  
#include "timer.h"		 	 	 	 
#include "string.h"	
#include "motor.h"
#include "encoder.h"	

typedef struct
{
	float Pdat;
	float pout;
	
	float Idat;
	float IMAX;
	float iout;
	
	float Ddat;
	float dout;
	
	float OUT;
	
	int error;
	int Last_error;	
}PID;

extern int Motor_Speed[4];			//电机速度值
extern int Motor_DrctAdj[4];		//电机转向调整

extern PID PID_Motor[4];
extern u8 ARMED;

void Car_Control(int X_Move,int Y_Move,int Yaw);
float Get_MxMi(float num,float max,float min);
void PID_Init(void);
float map(float val,float I_Min,float I_Max,float O_Min,float O_Max);
void Line_Control(void);

#endif

