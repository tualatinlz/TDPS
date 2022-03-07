/***************************************************************************
本程序可供学习使用，经作者允许，也可用于其他用途。如有意见及建议，欢迎来电交流指教
硬件版本：FreeLink STM32四路直流电机控制板V2.41
主控芯片：STM32F103RCT6
程序名称：STM32麦轮全向移动小车程序--OLED驱动程序
功能描述：驱动OLED显示字符、汉字、图像。

*****************************************************************************/



#include "oled_user.h"
#include "stdlib.h"
#include "Bitmap.h"
#include "cn_code.h" 
#include "en_code.h" 


unsigned char OLED_Buffer[8][128]={0}; 	//OLED显示缓存

//***************************屏幕刷新显示***************************
void OLED_Refresh_GDRAM(unsigned char displayxx)
{
	unsigned char disjj;
	OLED_Set_Pos(0,displayxx);
	for(disjj=0;disjj<128;disjj++)
	{
		OLED_WriteDat(OLED_Buffer[displayxx][disjj]);
	}	
}

//***************************全屏刷新显示***************************
void OLED_Refresh_AllGDRAM(void)
{
	OLED_Refresh_GDRAM(0);OLED_Refresh_GDRAM(1);
	OLED_Refresh_GDRAM(2);OLED_Refresh_GDRAM(3);
	OLED_Refresh_GDRAM(4);OLED_Refresh_GDRAM(5);
	OLED_Refresh_GDRAM(6);OLED_Refresh_GDRAM(7);	
}

//***************************显示Logo***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
void OLED_Draw_FreeLink(void)
{
	unsigned char i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<64;j++)
		{
			OLED_Buffer[i][j]=BMP_FreeLink[i][j];
		}
	}
}
//***************************显示12x24字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put12x24Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{  
	unsigned char  font_dat;
	unsigned char  i,j,k;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch=ch-0x20;
	for(k=0;k<3;k++)
	{
		for(i=0; i<12; i++)
		{   
			font_dat = ASCII12X24[ch][k*12+i];//读取字库里的数据
			for(j=0; j<8; j++)//写入8个点
			{  
				if(font_dat&0x80)	OLED_DrawDot(x+i, y+j+k*8, col0);   
				else 							OLED_DrawDot(x+i, y+j+k*8, col1); 
				font_dat=font_dat<<1;
			} 	
		}
	}
}
//***************************显示10x20字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put10x20Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{  
	unsigned char  font_dat;
	unsigned char  i,j,k;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch=ch-0x20;
	for(k=0;k<3;k++)
	{
		for(i=0; i<10; i++)
		{   
			font_dat = ASCII10X20[ch][k*10+i];//读取字库里的数据
			for(j=0; j<8; j++)//写入8个点
			{  
				if(font_dat&0x80)	OLED_DrawDot(x+i, y+j+k*8, col0);   
				else 							OLED_DrawDot(x+i, y+j+k*8, col1); 
				font_dat=font_dat<<1;
			} 	
		}
	}
}
//***************************显示8x16字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put8x16Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{  
	unsigned char  font_dat;
	unsigned char   i, j;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch=ch-0x20;
	for(i=0; i<16; i++)
	{   
			font_dat = ASCII8X16[ch][i];//读取字库里的数据
			for(j=0; j<8; j++)//写入8个点
			{  
				if(font_dat&0x80)	OLED_DrawDot(x+j, y+i, col0);   
				else 							OLED_DrawDot(x+j, y+i, col1); 
				font_dat=font_dat<<1;
			} 	
	}
}

//***************************显示7x14字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put7x14Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{  
	unsigned char  font_dat;
	unsigned char   i, j;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch=ch-0x20;
	for(i=0; i<14; i++)
	{   
			font_dat = ASCII7X14[ch][i];//读取字库里的数据
			for(j=0; j<7; j++)//写入7个点
			{  
				if(font_dat&0x80)	OLED_DrawDot(x+j, y+i, col0);   
				else 							OLED_DrawDot(x+j, y+i, col1); 
				font_dat=font_dat<<1;
			} 	
	}
}

//***************************显示6x12字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put6x12Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{ 
	u8 font_dat,j,i;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}
	 
	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20;   
	ch=ch-0x20;
	for(i=0; i<12; i++)
	{   
			font_dat = ASCII6X12[ch][i];//读取字库里的数据
			for(j=0; j<6; j++)//写入6个点
			{  
				if(font_dat&0x80)	OLED_DrawDot(x+j, y+i, col0);   
				else 							OLED_DrawDot(x+j, y+i, col1); 
				font_dat=font_dat<<1;
			} 	
	}
}
//***************************显示5x7字符***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//ch,	显示的字符
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put5x7Char(unsigned char x, unsigned char y, unsigned char ch,unsigned char Col)
{
	u8 temp,j,i;
	u16 y0=y;
	unsigned char col1,col0;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}
	 
	if( (ch<0x20) || (ch>0x7f) ) ch = 0x20; 
	ch=ch-0x20;

	for(i=0;i<5;i++)
	{   
		temp=	ASCII5x7[ch][i];
		y=y0;
		for(j=0;j<7;j++)
		{	
			if(temp&0x01)	OLED_DrawDot(x+i,y+j,col0);
			else 					OLED_DrawDot(x+i,y+j,col1);
			temp>>=1;
		}  	 
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//***************************显示24x24汉字、12x24字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//*Chstr,	显示的汉字、字符串，
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put24x24CNstr(u8 x,u8 y,u8 *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//是汉字
		{
			for(j=0;j<sizeof(hanzi_24x24)/sizeof(hanzi_24x24[0]);j++)  //
			{
					if(c1==hanzi_24x24[j].Index[0]&&c2==hanzi_24x24[j].Index[1]) break; //
			}
			for(i=0;i<24;i++)
			{

				tmp_char=hanzi_24x24[j].Msk[i*3];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_24x24[j].Msk[i*3+1];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				tmp_char=hanzi_24x24[j].Msk[i*3+2];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add+16,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=24;	
		}
		else //是英文字符
		{
			OLED_Put12x24Char(x,y,c1,Col);//显示英文字符
			m++;x+=12;
		}
	}
	while(m<length);
}
//***************************显示20x20汉字、12x24字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//*Chstr,	显示的汉字、字符串，
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put20x20CNstr(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//是汉字
		{
			for(j=0;j<sizeof(hanzi_20x20)/sizeof(hanzi_20x20[0]);j++)  //
			{
					if(c1==hanzi_20x20[j].Index[0]&&c2==hanzi_20x20[j].Index[1]) break; //
			}
			for(i=0;i<20;i++)
			{

				tmp_char=hanzi_20x20[j].Msk[i*3];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_20x20[j].Msk[i*3+1];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				tmp_char=hanzi_20x20[j].Msk[i*3+2];
				for (add=0;add<4;add++)
				{
					OLED_DrawDot(x+add+16,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=20;	
		}
		else //是英文字符
		{
			OLED_Put10x20Char(x,y,c1,Col);//显示英文字符
			m++;x+=10;
		}
	}
	while(m<length);
}
//***************************显示1616汉字、8x16字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//*Chstr,	显示的汉字、字符串，
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put16x16CNstr(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//
		{
			for(j=0;j<sizeof(hanzi_16x16)/sizeof(hanzi_16x16[0]);j++)  //
			{
					if(c1==hanzi_16x16[j].Index[0]&&c2==hanzi_16x16[j].Index[1]) break; //
			}
			for(i=0;i<16;i++)
			{

				tmp_char=hanzi_16x16[j].Msk[i*2];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_16x16[j].Msk[i*2+1];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=16;	
		}
		else //是英文字符
		{
			OLED_Put8x16Char(x,y,c1,Col);//显示英文字符
			m++;x+=8;
		}
	}
	while(m<length);
}

//***************************显示1414汉字、7x14字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//*Chstr,	显示的汉字、字符串，
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put14x14CNstr(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//
		{
			for(j=0;j<sizeof(hanzi_14x14)/sizeof(hanzi_14x14[0]);j++)  //
			{
					if(c1==hanzi_14x14[j].Index[0]&&c2==hanzi_14x14[j].Index[1]) break; //
			}
			for(i=0;i<14;i++)
			{

				tmp_char=hanzi_14x14[j].Msk[i*2];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_14x14[j].Msk[i*2+1];
				for (add=0;add<6;add++)
				{
					OLED_DrawDot(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=14;	
		}
		else //是英文字符
		{
			OLED_Put7x14Char(x,y,c1,Col);//显示英文字符
			m++;x+=7;
		}
	}
	while(m<length);
}

//***************************显示12x12汉字、6x12字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,	显示的X轴坐标
//y,	显示的Y轴坐标
//*Chstr,	显示的汉字、字符串，
//Col,显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put12x12CNstr(u8 x,u8 y,unsigned char *Chstr,u8 Col)
{
  unsigned char m,c1,c2,i,j,add,length;
	unsigned char tmp_char;
	unsigned char col1,col0;
	m=0;length=0;
	while(Chstr[length]!=0)length++;
	if(Col==LIGHT)	{	col0=LIGHT;col1=DARK;	}
	else						{	col0=DARK;col1=LIGHT;	}

	do
	{
		c1=Chstr[m];		
		c2=Chstr[m+1];   
		if(c1>128)//是汉字，显示汉字
		{
			//检索汉字在字库中的位置
			for(j=0;j<sizeof(hanzi_12x12)/sizeof(hanzi_12x12[0]);j++)  
			{
					if(c1==hanzi_12x12[j].Index[0]&&c2==hanzi_12x12[j].Index[1]) break;//找到对应的汉字
			}
			//显示汉字
			for(i=0;i<12;i++)
			{
				tmp_char=hanzi_12x12[j].Msk[i*2];
				for (add=0;add<8;add++)
				{
					OLED_DrawDot(x+add,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
				
				tmp_char=hanzi_12x12[j].Msk[i*2+1];
				for (add=0;add<4;add++)
				{
					OLED_DrawDot(x+add+8,y+i,(tmp_char&0x80) ? col0:col1);//写入颜色值
					tmp_char=tmp_char<<1;
				}
			}
      m+=2;x+=12;	
		}
		else //不是汉字，显示字符
		{
			OLED_Put6x12Char(x,y,c1,Col);//字符显示
			m++;
			x+=6;
		}
	}
	while(m<length);
}

//***************************显示5x7字符串***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//x,			显示的X轴坐标
//y,			显示的Y轴坐标
//*ENstr,	显示的汉字符串，
//Col,		显示的颜色，1,正显,字体白色，背景黑色；0,反显,字体黑色，背景白色
void OLED_Put5x7ENstr(u8 x,u8 y,unsigned char *ENstr,u8 Col)
{
	while(*ENstr!='\0')
	{
			OLED_Put5x7Char(x,y,*ENstr,Col);
			*ENstr++;
			x+=6;
	}
}

//***************************画点***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//xx,			显示的X轴坐标
//yy,			显示的Y轴坐标
//color,	显示的颜色，1,显示白色；0,显示黑色
void OLED_DrawDot(unsigned char xx,unsigned char yy,unsigned char color)
{
	unsigned char xlabel,xlabel_bit;
	unsigned char Read_Dat; 		//读LCD中的数据
	if(xx>=128) 	return;
	if(yy>=64) 	return;
	xlabel=yy>>3;								//取8*8首地址
	xlabel_bit=yy & 0x07;				//计算该点在16位数据的第几位	
	Read_Dat=OLED_Buffer[xlabel][xx];
	switch(color)
	{
		case 0:Read_Dat &= (~(0x01<<(xlabel_bit)));break;	//若变白 
		case 1:Read_Dat |= (0x01<<(xlabel_bit));break;		//若涂黑 
		case 2:Read_Dat ^= (0x01<<(xlabel_bit));break;		//若反转 
		default:break;
	}
	OLED_Buffer[xlabel][xx]=Read_Dat;
}

//***************************画横线***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X0,			横线的起始X轴坐标
//X1,			横线的结束Y轴坐标
//Y,			横线的Y轴坐标
//Color,	显示的颜色，1,显示白色；0,显示黑色
void OLED_DrawLine_X(unsigned char X0,unsigned char X1,unsigned char Y,unsigned char Color)//画 X轴水平线
{
	unsigned char temp;
	if (X0>X1)
	{
	   temp=X1;
	   X1=X0;
	   X0=temp;
	}
	for(;X0<X1;X0++)
	{
   	OLED_DrawDot(X0,Y,Color);
	}
}

//***************************画竖线***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X,			竖线的X轴坐标
//Y0,			竖线的起始Y轴坐标
//Y1,			竖线的结束Y轴坐标
//Color,	竖线的颜色，1,显示白色；0,显示黑色
void OLED_DrawLine_Y(unsigned char X,unsigned char Y0,unsigned char Y1,unsigned char Color)//画 Y轴水平线
{
	unsigned char temp;
	if (Y0>Y1)
	{
	   temp=Y1;
	   Y1=Y0;
	   Y0=temp;
	}

	for(;Y0<Y1;Y0++)
	{
   		OLED_DrawDot(X,Y0,Color);
	}
}

//***************************画任意线***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X,			线的起始X轴坐标
//X,			线的结束X轴坐标
//Y0,			线的起始Y轴坐标
//Y1,			线的结束Y轴坐标
//Color,	线的颜色，1,显示白色；0,显示黑色
void OLED_DrawLine(short int X0, short int Y0, short int X1 ,short int Y1, short int Color)
{
    short int t, distance;      /*根据屏幕大小改变变量类型(如改为int型)*/
    short int x = 0 , y = 0 , delta_x, delta_y ;
    short int incx, incy ;

    delta_x = X1 - X0 ;
    delta_y = Y1 - Y0 ;

    if( delta_x > 0 )
    {
        incx = 1;
    }
    else if( delta_x == 0 )
    {
        OLED_DrawLine_Y( X0, Y0, Y1, Color ) ;
        return ;
    }
    else
    {
        incx = -1 ;
    }
    if( delta_y > 0 )
    {
        incy = 1 ;
    }
    else if(delta_y == 0 )
    {
        OLED_DrawLine_X( X0, X1, Y0, Color ) ;    
        return ;
    }
    else
    {
        incy = -1 ;
    }

    delta_x = abs( delta_x );    
    delta_y = abs( delta_y );
    if( delta_x > delta_y )
    {
        distance = delta_x ;
    }
    else
    {
        distance = delta_y ; 
    }
    OLED_DrawDot( X0, Y0, Color ) ;    
		/* Draw Line*/
    for( t = 0 ; t <= distance+1 ; t++ )
    {
        OLED_DrawDot( X0, Y0, Color ) ;
        x += delta_x ;
        y += delta_y ;
        if( x > distance )
        {
           x -= distance ;
           X0 += incx ;
        }
        if( y > distance )
        {
           y -= distance ;
           Y0 += incy ;
        }
    }
}

//***************************画矩形***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X0,			矩形的左上端点X轴坐标
//Y0,			矩形的左上端点Y轴坐标
//X1,			矩形的右下端点X轴坐标
//Y1,			矩形的右下端点Y轴坐标
//Color,	矩形的颜色，1,显示白色；0,显示黑色
void OLED_DrawRectangle(unsigned char X0,unsigned char Y0,unsigned char X1,unsigned char Y1,unsigned char Color)//画矩形
{
	if(X0>X1 || Y0>Y1) return;//坐标不符合，直接返回
	OLED_DrawLine(X0,Y0,X1,Y0,Color);
	OLED_DrawLine(X0,Y1,X1,Y1,Color);
	OLED_DrawLine(X0,Y0,X0,Y1,Color);
	OLED_DrawLine(X1,Y0,X1,Y1,Color);
}

//***************************画圆***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X0,		圆心X轴坐标
//Y0,		圆心Y轴坐标
//R,	  圆的半径
void OLED_DrawCircle(short int X0,short int Y0,short int R)	  //画圆
{
	short int aa,bb;
	short int dii;
	if(R>31||R==0) return;
	aa=0;
	bb=R;
	dii=3-2*R;//判断下个点位置的标志
	while(aa<=bb)
	{
		OLED_DrawDot(X0-bb,Y0-aa,LIGHT);//3
		OLED_DrawDot(X0+bb,Y0-aa,LIGHT);//0
		OLED_DrawDot(X0-aa,Y0+bb,LIGHT);//1
		OLED_DrawDot(X0-bb,Y0-aa,LIGHT);//7
		OLED_DrawDot(X0-aa,Y0-bb,LIGHT);//2
		OLED_DrawDot(X0+bb,Y0+aa,LIGHT);//4
		OLED_DrawDot(X0+aa,Y0-bb,LIGHT);//5
		OLED_DrawDot(X0+aa,Y0+bb,LIGHT);//6
		OLED_DrawDot(X0-bb,Y0+aa,LIGHT);
		aa++;
//******使用bresenham算法画圆****/
		if(dii<0)
			dii+=4*aa+6;
		else
		{
			dii+=10+4*(aa-bb);
			bb--;
		}
		OLED_DrawDot(X0+aa,Y0+bb,LIGHT);
	}
}
//***************************矩形填充***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//X0,			矩形填充的左上端点X轴坐标
//Y0,			矩形填充的左上端点Y轴坐标
//X1,			矩形填充的右下端点X轴坐标
//Y1,			矩形填充的右下端点Y轴坐标
//Color,	矩形填充的颜色，1,显示白色；0,显示黑色
void OLED_FillRectangle(unsigned char X0,unsigned char Y0,unsigned char X1,unsigned char Y1,unsigned char Color)//矩形填充
{
	unsigned char i,j;
	if(X0>X1 || Y0>Y1) return;//坐标不符合，直接返回
	for(i=X0;i<X1;i++)
	{
		for(j=Y0;j<Y1;j++) 
			OLED_DrawDot(i,j,Color);
	}
}

//***************************清除显存***************************
//Col,	清除的颜色，1,显示白色；0,显示黑色
void OLED_ClearBuffer(unsigned char Col)
{
	unsigned char xxi,yyi;
	for(xxi=0;xxi<8;xxi++)
		for(yyi=0;yyi<128;yyi++)
			OLED_Buffer[xxi][yyi]=Col;
}

//***************************显示6x12多位数字***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//xx,			显示的X轴坐标
//yy,			显示的Y轴坐标
//Num,		显示的数字，
//bitlen,	显示的数字的位数，
//Col,		显示的颜色，1,显示白色；0,显示黑色
void OLED_Put6x12Num(unsigned char xx,unsigned char yy,int Num,unsigned char bitlen,u8 Col)
{
	unsigned char String[6],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;//数值是4位数
	if(Num>99) 		String[bitx++]=Num/100%10+48;	//数值是5位数
	if(Num>9) 		String[bitx++]=Num/10%10+48;	//数值是2位数
	String[bitx++]=Num%10+48;										//数值是1位数
	
	for(;bitx<bitlen;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_Put12x12CNstr(xx,yy,String,Col);
}

//***************************显示6x12带1位小数点的数字***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//xx,			显示的X轴坐标
//yy,			显示的Y轴坐标
//Num,		显示的数字，
//Col,		显示的颜色，1,显示白色；0,显示黑色
void OLED_Put6x12Num1dot(unsigned char xx,unsigned char yy,int Num,unsigned char Col)
{
	unsigned char String[7],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;	//数值是4位数
	if(Num>99) 		String[bitx++]=Num/100%10+48;		//数值是5位数
	String[bitx++]=Num/10%10+48;									//数值是2位数
	String[bitx++]='.';
	String[bitx++]=Num%10+48;											//数值是1位数
	
	for(;bitx<5;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_Put12x12CNstr(xx,yy,String,Col);
}

//***************************显示5x7带1位小数点的数字***************************
//注意：该函数只操作显示缓存的数据，不能直接在屏幕上显示！！！！！！！！！！！！！！
//调用OLED_Refresh_AllGDRAM()函数，全屏刷新后才能显示出来
//xx,			显示的X轴坐标
//yy,			显示的Y轴坐标
//Num,		显示的数字，
//Col,		显示的颜色，1,显示白色；0,显示黑色
void OLED_Put5x7Num1dot(unsigned char xx,unsigned char yy,int Num)
{
	unsigned char String[7],bitx=0;
	if(Num<0)
	{
		Num=-Num;
		String[bitx++]='-';
	}
	if(Num>999) 	String[bitx++]=Num/1000%10+48;	//数值是4位数
	if(Num>99) 		String[bitx++]=Num/100%10+48;	//数值是5位数
	String[bitx++]=Num/10%10+48;	//数值是2位数
	String[bitx++]='.';
	String[bitx++]=Num%10+48;		//数值是1位数
	
	for(;bitx<4;bitx++)
	{
		String[bitx]=' ';
	}
	String[bitx]='\0';
	
	OLED_Put5x7ENstr(xx,yy,String,LIGHT);
}

/*************************************END************************************/


