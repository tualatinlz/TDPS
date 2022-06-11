
#ifndef __ULN_H
#define __ULN_H	 
#include "sys.h"

#define Trig PCout(1)

extern float Distance1; //改了为flaot
extern float Distance_last;
void Uln_init(void);//外部中断初始化
void Uln_Trig(void);
#endif







