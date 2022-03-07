#ifndef __CONTROL_APP_H__
#define __CONTROL_APP_H__

int Is_Car_Front(const char *string);
int Is_Car_Back(const char *string);
int Is_Car_Left(const char *string);
int Is_Car_Right(const char *string);
int Is_Car_Stop(const char *string);
int Is_Duoji_Angle(const char *string);
void DJ_angle_control(const char *string);
void DJ_angle_control_1(const char *string);
void DJ_angle_control_2(const char *string);
void DJ_angle_control_3(const char *string);
void DJ_angle_control_4(const char *string);
void DJ_angle_control_5(const  char *string);
int Is_Duoji1_Angle(const char *string);
int Is_Duoji2_Angle(const char *string);
int Is_Duoji3_Angle(const char *string);
int Is_Duoji4_Angle(const char *string);
int Is_Duoji5_Angle(const char *string);

int Is_Car_horning(const char *string);
int Is_Car_Speed_Slow(const char *string);
int Is_Car_Speed_Add(const char *string);
int Is_Car_KD(const char *string);
int Is_LineControl(const char *string);
void App_control(const char *str);
void App_control_car(void);
void Servor_parse(const char *str);
int Is_Car_Nohorning(const char *string);
void OneLine(const char *str);
void LX_Control(const char *string);
void LY_Control(const char *string);
#endif

