/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--OLED驱动程序
功能描述：驱动4线OLED显示字符、汉字、图像。适用于0.96寸I2C接口的OLED


*****************************************************************************/



#include "oled.h"


//***********************************
//   功能：OLED初始化
//***********************************
void OLED_Init(void)
{
//I2C端口初始化
	RCC->APB2ENR|=1<<0;//使能辅助时钟
  RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟
		
	GPIOC->CRH &= 0XFFFF00FF;//PC11 10 推挽输出  
	GPIOC->CRH |= 0X00003300;	   
	GPIOC->ODR |= 0x3<<10;
	Delay_ms(100);
//OLED初始化	
	OLED_WriteCmd(0xae);//--turn off oled panel
	OLED_WriteCmd(0x00);//---set low column address
	OLED_WriteCmd(0x10);//---set high column address
	OLED_WriteCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WriteCmd(0x81);//--set contrast control register
	OLED_WriteCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WriteCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WriteCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WriteCmd(0xa6);//--set normal display
	OLED_WriteCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WriteCmd(0x3f);//--1/64 duty
	OLED_WriteCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WriteCmd(0x00);//-not offset
	OLED_WriteCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WriteCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WriteCmd(0xd9);//--set pre-charge period
	OLED_WriteCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WriteCmd(0xda);//--set com pins hardware configuration
	OLED_WriteCmd(0x12);
	OLED_WriteCmd(0xdb);//--set vcomh
	OLED_WriteCmd(0x40);//Set VCOM Deselect Level	 设置VCOMH 电压倍率
	OLED_WriteCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WriteCmd(0x02);//
	OLED_WriteCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WriteCmd(0x14);//--set(0x10) disable
	OLED_WriteCmd(0xa4);// Disable Entire Display On (0xa4/0xa5) 全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WriteCmd(0xa6);// Disable Inverse Display On (0xa6/a7) 设置显示方式;bit0:1,反相显示;0,正常显示
	OLED_WriteCmd(0xaf);//--turn on oled panel
	OLED_Fill(0x00);  //初始清屏
	OLED_Set_Pos(0,0);
}
//***********************************
//功能：OLED写数据
//***********************************
void OLED_WriteDat(unsigned char dat)
{
    OLED_I2C_Start();
    OLED_I2C_Send_Byte(0x78);
    OLED_I2C_Send_Byte(0x40);
    OLED_I2C_Send_Byte(dat);
    OLED_I2C_Stop();
}
//***********************************
//	功能：OLED写命令
//***********************************
void OLED_WriteCmd(unsigned char cmd)
{
	OLED_I2C_Start();
	OLED_I2C_Send_Byte(0x78);
	OLED_I2C_Send_Byte(0x00);
	OLED_I2C_Send_Byte(cmd);
	OLED_I2C_Stop();
}
//***********************************
//	功能OLED 设置坐标
//***********************************
void OLED_Set_Pos(unsigned char x,unsigned char y)
{
	OLED_WriteCmd(0xb0+y);
	OLED_WriteCmd(((x&0xf0)>>4)|0x10);
	OLED_WriteCmd( x&0x0f);
}

//********************以下是I2C接口驱动代码*************************
void OLED_I2C_Start(void)
{
    OLED_SDA_OUT();     //SDA设置为输出
    OLED_I2C_SDA=1;        
    OLED_I2C_SCL=1;
// 		delay_us(1);
    OLED_I2C_SDA=0;//START:when CLK is high,DATA change form high to low 
// 		delay_us(1);
    OLED_I2C_SCL=0;
}     
//
void OLED_I2C_Stop(void)
{
    OLED_SDA_OUT();//SDA设置为输出
    OLED_I2C_SCL=0;
    OLED_I2C_SDA=0;//STOP:when CLK is high DATA change form low to high
// 		delay_us(1);
    OLED_I2C_SCL=1; 
    OLED_I2C_SDA=1;
// 		delay_us(1);                                
}

u8 OLED_I2C_Wait_Ack(void)
{
    u8 ucErrTime=0;
    OLED_SDA_IN();      //SDA设置为输入
    OLED_I2C_SDA=1;//delay_us(1);     
    OLED_I2C_SCL=1;//delay_us(1);   
    while(READ_OLED_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            OLED_I2C_Stop();
            return 1;
        }
    }
    OLED_I2C_SCL=0;   
    return 0;  
} 

void OLED_I2C_Send_Byte(u8 txd)
{                        
    u8 t;   
    OLED_SDA_OUT(); //SDA设置为输出      
    OLED_I2C_SCL=0;//
    for(t=0;t<8;t++)
    {              
        OLED_I2C_SDA=(txd&0x80)>>7;
        txd<<=1;    
//         delay_us(1);   
        OLED_I2C_SCL=1;
//         delay_us(1); 
        OLED_I2C_SCL=0;  
    }    
    OLED_I2C_Wait_Ack();
} 



//***********************************
//功能：OLED全屏
//***********************************
void OLED_Fill(unsigned char bmp_dat)
{
	unsigned char y,x;
	for(y=0;y<8;y++)
	{
		OLED_WriteCmd(0xb0+y);
		OLED_WriteCmd(0x01);
		OLED_WriteCmd(0x10);
		for(x=0;x<X_WIDTH;x++)
		OLED_WriteDat(bmp_dat);
	}
}


/*************************************END************************************/



