#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 


void USART_Config(USART_TypeDef* TUSARTx,u32 bound); 
void Uart_Init(u16 uart_num);
void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data);
void UART_PutStr (USART_TypeDef* USARTx, char *str);
//void UART_Put_Num(uint16 dat);
//void UART_Put_Inf(char *inf,uint16 dat);
void USART1_Config(u32 bound);
void Uart1WriteBuf(uint8 *buf, uint8 len);
#endif



