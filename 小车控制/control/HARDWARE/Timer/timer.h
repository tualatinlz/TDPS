#ifndef TIMER_H
#define TIMER_H

#include "stm32f10x.h"
#include "sys.h"
void Timer_Init(void);
void Timer_ON(void);
void Timer_OFF(void);
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM7_Int_Init(u16 arr,u16 psc);

#endif	/* TIME_TEST_H */
