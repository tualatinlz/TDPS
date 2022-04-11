#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#define TRUE	1
#define FALSE	0

//IO定义
//IO方向设置
#define MPU_SDA_IN()  {GPIOC->CRH&=0XFF0FFFFF;GPIOC->CRH|=0X00800000;}
#define MPU_SDA_OUT() {GPIOC->CRH&=0XFF0FFFFF;GPIOC->CRH|=0X00300000;}

#define MPU_SCL_H         PCout(12)=1
#define MPU_SCL_L         PCout(12)=0
    
#define MPU_SDA_H         PCout(13)=1
#define MPU_SDA_L         PCout(13)=0

#define MPU_SCL_read      PCin(12) 
#define MPU_SDA_read      PCin(13)


//IIC所有操作函数
void MPU_I2C_Init_IO(void);       		//初始化IIC的IO口				 
void MPU_I2C_Start(void);							//发送IIC开始信号
void MPU_I2C_Stop(void);	  					//发送IIC停止信号

unsigned char MPU_I2C_WaitAck(void);	//IIC等待ACK信号
void MPU_I2C_Ack(void);								//IIC发送ACK信号
void MPU_I2C_NoAck(void);							//IIC不发送ACK信号

void MPU_I2C_SendByte(u8 SendByte);		//IIC发送一个字节
u8 MPU_I2C_ReadByte(void);          	//IIC读取一个字节

void MPU_I2C_Start2(void);						//发送IIC开始信号
void MPU_I2C_Stop2(void);	  					//发送IIC停止信号

unsigned char MPU_I2C_WaitAck2(void);	//IIC等待ACK信号
void MPU_I2C_Ack2(void);							//IIC发送ACK信号
void MPU_I2C_NoAck2(void);						//IIC不发送ACK信号

void MPU_I2C_SendByte2(u8 SendByte);	//IIC发送一个字节
u8 MPU_I2C_ReadByte2(void);           //IIC读取一个字节

//******单字节写入**********//
unsigned char MPU_Single_Write(u8 REG_Address,u8 REG_data,u8 SlaveAddress);
unsigned char MPU_Single_Write2(u8 REG_Address,u8 REG_data,u8 SlaveAddress);
//********单字节读取*********//
u8 MPU_Single_Read(u8 REG_Address,u8 SlaveAddress);
//多字节写入//
void MPU_Multiple_write(u8 star_addr,u8 num,u8 SlaveAddress,u8* send_buf);
//读取多个字节的数据//
void MPU_Multiple_read(u8 star_addr,u8 num,u8 SlaveAddress,u8* recv_buf);

/////////////////////////////mpu6050接口函数///////////////////////////////////////////////////////////
// void i2cRead(u8 SlaveAddress,u8 star_addr,u8 num,u8* recv_buf);
// void i2cWrite(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
  
#endif
















