#include "sys.h"
#include "usart.h"	

unsigned char flag_RecFul=0;
char redata[500] = {0};    // 定义接收数据变量数组

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 




void Uart1_NVIC_Init(void)
{
	  //Usart1 NVIC 配置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

void Uart2_NVIC_Init(void)
{
	  //Usart2 NVIC 配置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
void Uart3_NVIC_Init(void)
{
	  //Usart1 NVIC 配置
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}

void Uart1_Gpio_Config(void)
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
		//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

}

void Uart2_Gpio_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);	//使能USART2，GPIOA时钟
		//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART2_RX	  GPIOA.3初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

}
void Uart3_Gpio_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能USART2，GPIOA时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);	//使能USART2，GPIOA时钟
		//USART3_TX   GPIOB.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.10
   
  //USART3_RX	  GPIOB.11初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB.11  

}

void Uart_Init(u16 uart_num)
{
	if(uart_num==1)
	{
		Uart1_NVIC_Init();
		Uart1_Gpio_Config();
		
	}
	else if(uart_num==2)
	{
		Uart2_NVIC_Init();
		Uart2_Gpio_Config();
	}
	else if(uart_num==3)
	{
		Uart3_NVIC_Init();
		Uart3_Gpio_Config();  
	}
}

void USART_Config(USART_TypeDef* TUSARTx,u32 bound) 
{
  
	USART_InitTypeDef USART_InitStructure;
 
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(TUSARTx, &USART_InitStructure); //初始化串口
  USART_ITConfig(TUSARTx, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(TUSARTx, ENABLE);                    //使能串口

}

void USART1_Config(u32 bound)
{

  USART_InitTypeDef USART_InitStructure;   
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
		
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
//	USART_HalfDuplexCmd(USART1,ENABLE);
	USART_Cmd(USART1, ENABLE); 
}



void UART_PutChar(USART_TypeDef* USARTx, uint8_t Data)  
{  
    USART_SendData(USARTx, Data);  
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET){}  
}  



void UART_PutStr (USART_TypeDef* USARTx, char *str) 
{
	unsigned int i = 0;
	while(*str != '\0' && i < 500)
	{
		UART_PutChar(USARTx, *str++); 
		i++;
	}
}

void Uart1WriteBuf(uint8 *buf, uint8 len)
{
	while (len--) {
		while ((USART1->SR & 0x40) == 0);
		USART_SendData(USART1,*buf++);
	}
}
/*
void UART_Put_Num(uint16  dat)
{
 char temp[20]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
 u32tostr(dat,temp);
 UART_PutStr(USART1,temp); 
}

void UART_Put_Inf(char *inf,uint16 dat)
{
 UART_PutStr (USART1,inf); 
 UART_Put_Num(dat);
 UART_PutStr (USART1,"\n");  
}
*/
void USART1_IRQHandler(void)                
	{

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  
	{ 
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		
	 } 
} 

void USART2_IRQHandler(void) 
{

		if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  
		{ 
			USART_ClearITPendingBit(USART2,USART_IT_RXNE);

		}
}


void USART3_IRQHandler(void)                
	{
  static unsigned int i=0;
	u8 Res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{ 
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		Res =USART_ReceiveData(USART3);	
					
		if(!flag_RecFul)
		{
			redata[i]=Res;
			i++;
		
		if(redata[i-1]=='\r'||redata[i-1]=='>'||i>500)
		{
			  redata[i] = 0;
			  flag_RecFul=1;
				i=0;
			}
		}
		else
		{

    }
		
	 } 
} 






