#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"
#define TRUE	1
#define FALSE	0

//IO����
//IO��������
#define MPU_SDA_IN()  {GPIOC->CRH&=0XFF0FFFFF;GPIOC->CRH|=0X00800000;}
#define MPU_SDA_OUT() {GPIOC->CRH&=0XFF0FFFFF;GPIOC->CRH|=0X00300000;}

#define MPU_SCL_H         PCout(12)=1
#define MPU_SCL_L         PCout(12)=0
    
#define MPU_SDA_H         PCout(13)=1
#define MPU_SDA_L         PCout(13)=0

#define MPU_SCL_read      PCin(12) 
#define MPU_SDA_read      PCin(13)


//IIC���в�������
void MPU_I2C_Init_IO(void);       		//��ʼ��IIC��IO��				 
void MPU_I2C_Start(void);							//����IIC��ʼ�ź�
void MPU_I2C_Stop(void);	  					//����IICֹͣ�ź�

unsigned char MPU_I2C_WaitAck(void);	//IIC�ȴ�ACK�ź�
void MPU_I2C_Ack(void);								//IIC����ACK�ź�
void MPU_I2C_NoAck(void);							//IIC������ACK�ź�

void MPU_I2C_SendByte(u8 SendByte);		//IIC����һ���ֽ�
u8 MPU_I2C_ReadByte(void);          	//IIC��ȡһ���ֽ�

void MPU_I2C_Start2(void);						//����IIC��ʼ�ź�
void MPU_I2C_Stop2(void);	  					//����IICֹͣ�ź�

unsigned char MPU_I2C_WaitAck2(void);	//IIC�ȴ�ACK�ź�
void MPU_I2C_Ack2(void);							//IIC����ACK�ź�
void MPU_I2C_NoAck2(void);						//IIC������ACK�ź�

void MPU_I2C_SendByte2(u8 SendByte);	//IIC����һ���ֽ�
u8 MPU_I2C_ReadByte2(void);           //IIC��ȡһ���ֽ�

//******���ֽ�д��**********//
unsigned char MPU_Single_Write(u8 REG_Address,u8 REG_data,u8 SlaveAddress);
unsigned char MPU_Single_Write2(u8 REG_Address,u8 REG_data,u8 SlaveAddress);
//********���ֽڶ�ȡ*********//
u8 MPU_Single_Read(u8 REG_Address,u8 SlaveAddress);
//���ֽ�д��//
void MPU_Multiple_write(u8 star_addr,u8 num,u8 SlaveAddress,u8* send_buf);
//��ȡ����ֽڵ�����//
void MPU_Multiple_read(u8 star_addr,u8 num,u8 SlaveAddress,u8* recv_buf);

/////////////////////////////mpu6050�ӿں���///////////////////////////////////////////////////////////
// void i2cRead(u8 SlaveAddress,u8 star_addr,u8 num,u8* recv_buf);
// void i2cWrite(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
  
#endif
















