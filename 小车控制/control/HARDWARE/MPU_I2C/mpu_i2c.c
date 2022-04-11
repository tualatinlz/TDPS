#include "mpu_i2c.h"
#include "Delay.h"

//初始化IIC
void MPU_I2C_Init_IO(void)
{					     
 	RCC->APB2ENR|=1<<4;//先使能外设IO PORTC时钟 							 
	GPIOC->CRH &= 0XFF00FFFF;//PC12,13 推挽输出
	GPIOC->CRH |= 0X00330000;	   
	GPIOC->ODR |= 3<<4;     //PC12,13 输出高
}
//产生IIC起始信号
void MPU_I2C_Start(void)
{
	MPU_SDA_OUT();     //sda线输出
	MPU_SDA_H;MPU_SDA_H;
	MPU_SCL_H;MPU_SCL_H;
	MPU_SDA_L;MPU_SDA_L;
	MPU_SCL_L;MPU_SCL_L;
}

//产生IIC停止信号
void MPU_I2C_Stop(void)
{
	MPU_SDA_OUT();     //sda线输出
	MPU_SCL_L;MPU_SCL_L;
	MPU_SDA_L;MPU_SDA_L;
	MPU_SCL_H;MPU_SCL_H;
	MPU_SDA_H;MPU_SDA_H;
} 

//等待应答信号到来
//返回为:=1有ACK,=0无ACK
unsigned char MPU_I2C_WaitAck(void) 	 
{
	u8 errtime = 0;
	MPU_SCL_L;
	MPU_SDA_H;MPU_SDA_H;//拉高数据总线准备读数据	
	MPU_SDA_IN();      //SDA设置为输入 	
	MPU_SCL_H;MPU_SCL_H;
	while(MPU_SDA_read)
	{
		errtime++;
		if(errtime>250)
		{
			MPU_I2C_Stop();
			return FALSE;	
		}
	}
	MPU_SCL_L;//时钟拉低
	return TRUE;
}

//产生ACK应答
void MPU_I2C_Ack(void)
{	
	MPU_SCL_L;MPU_SCL_L;
	MPU_SDA_OUT();
	MPU_SDA_L;MPU_SDA_L;
	MPU_SCL_H;MPU_SCL_H;
	MPU_SCL_L;MPU_SCL_L;
}

//不产生ACK应答
void MPU_I2C_NoAck(void)
{	
	MPU_SCL_L;MPU_SCL_L;
	MPU_SDA_OUT();
	MPU_SDA_H;MPU_SDA_H;
	MPU_SCL_H;MPU_SCL_H;
	MPU_SCL_L;MPU_SCL_L;
}

//IIC发送一个字节
//数据从高位到低位//
void MPU_I2C_SendByte(u8 SendByte)
{
	u8 i;
	MPU_SDA_OUT();
	for(i=0;i<8;i++)
	{
		MPU_SCL_L;
		if(SendByte&(0x80>>i)) MPU_SDA_H;  
		else MPU_SDA_L;   
		MPU_SCL_H;
	}
	MPU_SCL_L;
}

//读1个字节 
//数据从高位到低位// 
u8 MPU_I2C_ReadByte(void)  
{ 
	u8 i=0;
	u8 ReceiveByte=0;
	MPU_SDA_IN();//SDA设置为输入			
	for(i=0;i<8;i++)
	{      
		MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;MPU_SCL_L;
		MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;MPU_SCL_H;
		ReceiveByte	<<= 1;
		if(MPU_SDA_read)	ReceiveByte|=0x01; 	
	}
	MPU_SCL_L;
  return ReceiveByte;
}
#define MPU_I2C_DELAY	2
//////////////////////////////////////////////
void MPU_I2C_Start2(void)
{
    MPU_SDA_OUT();     //sda???
    MPU_SDA_H;
    MPU_SCL_H;delay_us(MPU_I2C_DELAY);
    MPU_SDA_L;delay_us(MPU_I2C_DELAY);
    MPU_SCL_L;delay_us(MPU_I2C_DELAY);
}
void MPU_I2C_Stop2(void)
{
    MPU_SDA_OUT();     //sda???
    MPU_SCL_L;delay_us(MPU_I2C_DELAY);
    MPU_SDA_L;delay_us(MPU_I2C_DELAY);
    MPU_SCL_H;delay_us(MPU_I2C_DELAY);
    MPU_SDA_H;delay_us(MPU_I2C_DELAY);
} 
unsigned char MPU_I2C_WaitAck2(void) 	 
{
  unsigned char errtime = 0;
  MPU_SDA_IN();      //SDA????? 
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
  MPU_SDA_H;delay_us(MPU_I2C_DELAY);
  MPU_SCL_H;delay_us(MPU_I2C_DELAY);
  while(MPU_SDA_read)
  {
    errtime++;
    if(errtime>250)
    {
        MPU_I2C_Stop2();
        return FALSE;	
    }
  }
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
  return TRUE;
}
//??ACK??
void MPU_I2C_Ack2(void)
{	
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
  MPU_SDA_OUT();
  MPU_SDA_L;delay_us(MPU_I2C_DELAY);
  MPU_SCL_H;delay_us(MPU_I2C_DELAY);
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
}
//???ACK??
void MPU_I2C_NoAck2(void)
{	
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
  MPU_SDA_OUT();
  MPU_SDA_H;delay_us(MPU_I2C_DELAY);
  MPU_SCL_H;delay_us(MPU_I2C_DELAY);
  MPU_SCL_L;delay_us(MPU_I2C_DELAY);
}
//IIC??????
//????????//
void MPU_I2C_SendByte2(unsigned char SendByte)
{
    unsigned char i;
    MPU_SDA_OUT();   
    for(i=0;i<8;i++)
    {
        MPU_SCL_L;delay_us(MPU_I2C_DELAY);
      	if(SendByte&(0x80>>i)) MPU_SDA_H; 
      	else MPU_SDA_L;
				MPU_SCL_H;delay_us(MPU_I2C_DELAY);
    }
    MPU_SCL_L;delay_us(MPU_I2C_DELAY);
}
//?1??? 
//????????// 
unsigned char MPU_I2C_ReadByte2(void)  
{ 
    unsigned char i=0;
    unsigned char ReceiveByte=0;
    MPU_SDA_IN();//SDA?????			
    for(i=0;i<8;i++)
    {      
      MPU_SCL_L;delay_us(MPU_I2C_DELAY);
      MPU_SCL_H;delay_us(MPU_I2C_DELAY);
      ReceiveByte<<= 1;
      if(MPU_SDA_read)
      {
         ReceiveByte++; 
      }
    }
    MPU_SCL_L;delay_us(MPU_I2C_DELAY);
    return ReceiveByte;
}

//******单字节写入**********//
unsigned char Single_Write(u8 REG_Address,u8 REG_data,u8 SlaveAddress)
{
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress);//发送设备地址+写信号
	while(!MPU_I2C_WaitAck());
	MPU_I2C_SendByte(REG_Address);//设置低起始地址
	while(!MPU_I2C_WaitAck());
	MPU_I2C_SendByte(REG_data);
	while(!MPU_I2C_WaitAck());
	MPU_I2C_Stop();
	delay_ms(5);
	return TRUE;	
}
unsigned char Single_Write2(u8 REG_Address,u8 REG_data,u8 SlaveAddress)
{
	MPU_I2C_Start2();
	MPU_I2C_SendByte2(SlaveAddress);//发送设备地址+写信号
	while(!MPU_I2C_WaitAck2());
	MPU_I2C_SendByte2(REG_Address);//设置低起始地址
	MPU_I2C_WaitAck2();
	MPU_I2C_SendByte2(REG_data);
	MPU_I2C_WaitAck2();
	MPU_I2C_Stop2();
	delay_ms(5);
	return TRUE;	
}
//********单字节读取*********//
u8 Single_Read(u8 REG_Address,u8 SlaveAddress)
{
	u8 data=0;
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress);//发送设备地址+写信号
	while(!MPU_I2C_WaitAck());
	MPU_I2C_SendByte(REG_Address);   //设置低起始地址
	while(!MPU_I2C_WaitAck()); 
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress+1);//发送设备地址+读信号
	while(!MPU_I2C_WaitAck());

	data = MPU_I2C_ReadByte();
	MPU_I2C_NoAck();
   MPU_I2C_Stop();
	return data;	
}

//多字节写入//
void Multiple_write(u8 star_addr,u8 num,u8 SlaveAddress,u8* send_buf)
{
	u8 i;
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress);//发送设备地址
	while(!MPU_I2C_WaitAck());
	MPU_I2C_SendByte(star_addr);//发送起始地址
	while(!MPU_I2C_WaitAck());
	for(i=0;i<num;i++)
	{
		MPU_I2C_SendByte(send_buf[i]);
		while(!MPU_I2C_WaitAck());
	}
	MPU_I2C_Stop();
}

//读取多个字节的数据
void Multiple_read(u8 star_addr,u8 num,u8 SlaveAddress,u8* recv_buf)
{
	u8 i;
	
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress);//发送设备地址
	while(!MPU_I2C_WaitAck());
	MPU_I2C_SendByte(star_addr);//发送起始地址
	while(!MPU_I2C_WaitAck());
	
	MPU_I2C_Start();
	MPU_I2C_SendByte(SlaveAddress+1);//发送设备地址+读信号
	while(!MPU_I2C_WaitAck());
	
	for(i=0;i<num;i++)
	{
		recv_buf[i] = MPU_I2C_ReadByte();
		if(i == (num-1)) MPU_I2C_NoAck();//最后一个数据需要回NOACK
		else MPU_I2C_Ack();
	}
	MPU_I2C_Stop();//停止信号
}




///////////////////////////////////////////MPU6050接口函数////////////////////////////////////////////////////////
// void i2cRead(u8 SlaveAddress,u8 star_addr,u8 num,u8* recv_buf)
// {
// 	Multiple_read(star_addr,num,SlaveAddress,recv_buf);
// }

// void i2cWrite(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
// {
// 	Single_Write(REG_Address,REG_data,SlaveAddress);
// }
