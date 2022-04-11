#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void Delay_us(u32 nTime);
void Delay_ms(u32 nTime);  
void TimingDelay_Decrement(void);
void delay_init(u8 SYSCLK);
void delay_us(u32 nus);
void delay_ms(u16 nms);
#endif
