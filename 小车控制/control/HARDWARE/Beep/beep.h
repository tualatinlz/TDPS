#ifndef __BEEP_H__
#define __BEEP_H__
#include "sys.h"


#define BEEP(a) if(a) \
											GPIO_SetBits(GPIOB,GPIO_Pin_12); \
								else \
											GPIO_ResetBits(GPIOB,GPIO_Pin_12)
								
#define	Beep PBout(12) 									

void Beep_Init(void);
void Beep_Test(void);
void N_Beep(char n);
#endif
