#ifndef __SERVOR_H
#define	__SERVOR_H

#include "stm32f10x.h"
#include "sys.h"

void Servo1(void);
void Servor_GPIO_Config(void);	
void ServoSetPluseAndTime(unsigned int id,unsigned int p,unsigned int time);
void ServoPwmDutyCompare(void);
bool ServoAction(void);


#endif /* __GPIO_H */
