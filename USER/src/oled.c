/***********************************************************************************************************************
Copyright 2008 - 2016 深圳市信盈达电子有限公司. All rights reserved.
文件名:        oled.c
描述   :       oled驱动
作者   :        
版本   :       V1.0
修改   :   
完成日期：      2016.10.13
信盈达官网：http://www.edu118.com/
信盈达网校：http://www.edu118.cn/
Notice    :本程序只供学习使用，未经作者许可，不得用于其它任何用途。版权所有，盗版必究。
************************************************************************************************************************/

/***********************************************************************************************************************
 * INCLUDES
 */
#include "oled.h"
#include "spi.h"
#include "font.h"
#include "delay.h"
/***********************************************************************************************************************
 * CONSTANTS
 */


/***********************************************************************************************************************
 * TYPEDEFS
 */


/***********************************************************************************************************************
 * LOCAL VARIABLES
 */
 
/***********************************************************************************************************************
 * LOCAL FUNCTIONS  DECLARE
 */


/***********************************************************************************************************************
 * LOCAL FUNCTIONS  
 */



/***********************************************************************************************************************
 * PUBLIC FUNCTIONS
 */
/**********************************************************************************************************
* 函数名    ：   Oled_init
* 功能描述  ：   oled初始化
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/


//初始化SSD1306					    
void Oled_init(void)
{ 	 
	OLED_pinInit();	
  OLED_deSelect();  //不选择OLED
	OLED_reset();		
	
	OLED_writeByte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_writeByte(0x02,OLED_CMD);//---SET low column address
	OLED_writeByte(0x10,OLED_CMD);//---SET high column address
	OLED_writeByte(0x40,OLED_CMD);//--SET start line address  SET Mapping RAM Display Start Line (0x00~0x3F)
	OLED_writeByte(0x81,OLED_CMD);//--SET contrast control register
	OLED_writeByte(0xCF,OLED_CMD); // SET SEG Output Current Brightness
	OLED_writeByte(0xA1,OLED_CMD);//--SET SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_writeByte(0xC8,OLED_CMD);//SET COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_writeByte(0xA6,OLED_CMD);//--SET normal display
	OLED_writeByte(0xA8,OLED_CMD);//--SET multiplex ratio(1 to 64)
	OLED_writeByte(0x3f,OLED_CMD);//--1/64 duty
	OLED_writeByte(0xD3,OLED_CMD);//-SET display offSET	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_writeByte(0x00,OLED_CMD);//-not offSET
	OLED_writeByte(0xd5,OLED_CMD);//--SET display clock divide ratio/oscillator frequency
	OLED_writeByte(0x80,OLED_CMD);//--SET divide ratio, SET Clock as 100 Frames/Sec
	OLED_writeByte(0xD9,OLED_CMD);//--SET pre-charge period
	OLED_writeByte(0xF1,OLED_CMD);//SET Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_writeByte(0xDA,OLED_CMD);//--SET com pins hardware configuration
	OLED_writeByte(0x12,OLED_CMD);
	OLED_writeByte(0xDB,OLED_CMD);//--SET vcomh
	OLED_writeByte(0x40,OLED_CMD);//SET VCOM Deselect Level
	OLED_writeByte(0x20,OLED_CMD);//-SET Page Addressing Mode (0x00/0x01/0x02)
	OLED_writeByte(0x02,OLED_CMD);//
	OLED_writeByte(0x8D,OLED_CMD);//--SET Charge Pump enable/disable
	OLED_writeByte(0x14,OLED_CMD);//--SET(0x10) disable
	OLED_writeByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_writeByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_writeByte(0xAF,OLED_CMD);//--turn on oled panel
	
	OLED_writeByte(0xAF,OLED_CMD); /*display ON*/ 
	OLED_clear();
	OLED_setPos(0,0); 	
}  
/**********************************************************************************************************
* 函数名    ：   OLED_Reset
* 功能描述  ：   oled复位
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
u8 OLED_reset(void)
{
	OLED_RST_CLR();
	delay_ms(25);   //  ：奇怪的问题，延时改长了，就不能兼容HP-6
	OLED_RST_SET();
	return 0;       //返回0，表示复位成功
}
/**********************************************************************************************************
* 函数名    ：   OLED_resetPinInit
* 功能描述  ：   oled复位管脚初始化
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
								OLED_RES  	PB13
								OLED_DC 		PA15
								OLED_CS  		PB7
**********************************************************************************************************/
u8 OLED_pinInit(void)
{
	RCC->AHB1ENR |= 3<<0;//PA  PB
	
	//OLED_RES  	PB13
	GPIOB->MODER &= ~(3<<26);
	GPIOB->MODER |= 1<<26;
	
	GPIOB->OTYPER &= ~(1<<13);
	GPIOB->OSPEEDR &= ~(3<<26);
	GPIOB->OSPEEDR |= 2<<26;
	
	//OLED_DC 		PA15
	GPIOA->MODER &= ~(3ul<<30);
	GPIOA->MODER |= 1ul<<30;
	
	GPIOA->OTYPER &= ~(1<<15);
	GPIOA->OSPEEDR &= ~(3ul<<30);
	GPIOA->OSPEEDR |= 2ul<<30;
	
	//OLED_CS  		PB7
	GPIOB->MODER &= ~(3<<14);
	GPIOB->MODER |= 1<<14;
	
	GPIOB->OTYPER &= ~(1<<7);
	GPIOB->OSPEEDR &= ~(3<<14);
	GPIOB->OSPEEDR |= 2<<14;	
	
	return 0;
}
/**********************************************************************************************************
* 函数名    ：   OLED_select
* 功能描述  ：   oled片选
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_select(void)
{
   OLED_CS_CLR();//拉低使能端
}
/**********************************************************************************************************
* 函数名    ：   OLED_deSelect
* 功能描述  ：   oled取消片选
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_deSelect(void)
{  
	 OLED_CS_SET();//拉高使能端
}


/**********************************************************************************************************
* 函数名    ：   OLED_writeByte
* 功能描述  ：   oled字节写，向SSH1106写入一个字节。
* 作者      ：      
* 参数说明  ：   dat:要写入的数据/命令
*                cmd:数据/命令标志 0,表示命令;1,表示数据;
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_writeByte(u8 dat, u8 cmd)
{	
	cmd ? OLED_DC_SET() : OLED_DC_CLR();	
		
	OLED_select();		 
	
	Spi1_RevSendByte(dat);
	
	OLED_deSelect();
} 
/**********************************************************************************************************
* 函数名    ：   OLED_clear
* 功能描述  ：   清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_writeByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_writeByte (0x02,OLED_CMD);      //设置显示位置—列低地址
		OLED_writeByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0; n<128; n++)
		{
			OLED_writeByte(0,OLED_DATA); 
		}
	} //更新显示
}
/**********************************************************************************************************
* 函数名    ：   OLED_setPos
* 功能描述  ：   清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/
void OLED_setPos(unsigned char x, unsigned char y) 
{ 
	x += 2;//SSH1106 要加这个
	
	OLED_writeByte(0xb0+y,OLED_CMD);
	OLED_writeByte(((x&0xf0)>>4)|0x10, OLED_CMD);
	OLED_writeByte((x&0x0f)|0x01,OLED_CMD); 
}  
/**********************************************************************************************************
* 函数名    ：   OLED_displayOn
* 功能描述  ：   开启OLED显示  
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/   
void OLED_displayOn(void)
{
	OLED_writeByte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_writeByte(0X14,OLED_CMD);  //DCDC ON
	OLED_writeByte(0XAF,OLED_CMD);  //DISPLAY ON
}
/**********************************************************************************************************
* 函数名    ：   OLED_displayOff
* 功能描述  ：   关闭OLED显示   
* 作者      ：      
* 参数说明  ：   none
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/   
void OLED_displayOff(void)
{
	OLED_writeByte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_writeByte(0X10,OLED_CMD);  //DCDC OFF
	OLED_writeByte(0XAE,OLED_CMD);  //DISPLAY OFF
}	

/*******************************************************************************
* 函数名：OLED_showChar
* 功能描述：在指定位置显示一个字符,包括部分字符
* 作者： 	
* 参数说明：  
*			在指定位置显示一个字符,包括部分字符
*			x:0~127
*			y:0~63
*			mode:0,反白显示;1,正常显示				 
*			size:选择字体 24/16/12
* 返回值说明：none
* 修改记录：
*******************************************************************************/

void OLED_showChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
  u8 c=0,i=0;	
  c=chr-' ';//得到偏移后的值			
  if(x>MAX_COLUMNC-1)
  {
    x=0;
    y=y+2;  //这个要优化，根据字体的不同来换行
  }
  if(Char_Size == 24)
  {
    OLED_setPos(x,y);	
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i],OLED_DATA);
    OLED_setPos(x,y+1);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16],OLED_DATA);
		OLED_setPos(x,y+2);
    for(i=0;i<16;i++)
      OLED_writeByte(F16X24[c*48+i+16*2],OLED_DATA);
  }
  else if(Char_Size ==16)
  {
    OLED_setPos(x,y);	
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i],OLED_DATA); //一个字符用16个字节来控制
    OLED_setPos(x,y+1); //y+1 表示换行，
    for(i=0;i<8;i++)
      OLED_writeByte(F8X16[c*16+i+8],OLED_DATA);
  }
	
  else 
  {	
    OLED_setPos(x,y);
    for(i=0;i<6;i++)
      OLED_writeByte(F6x8[c][i],OLED_DATA); 
  }
}


/*******************************************************************************
* 函数名：OLED_showString
* 功能描述：显示一个字符号串
* 作者： 	
* 参数说明：  
* 返回值说明：none
* 修改记录：
*******************************************************************************/
void OLED_showString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
  u8 j=0;
  while (chr[j]!='\0')
  {		
    OLED_showChar(x,y,chr[j],Char_Size);
		if(24 == Char_Size)
		{
		  x+=16;
		}
		else
      x+=8;
    if(x>120){x=0;y+=2;}
    j++;
  }
}



/*******************************************************************************
* 函数名：OLED_showNum
* 功能描述：显示2个数字
* 作者： 	
* 参数说明：  
*			x,y :起点坐标	 
*		    en :数字的位数
*		    size:字体大小
*			mode:模式	0,填充模式;1,叠加模式
*			num:数值(0~4294967295);	
* 返回值说明：none
* 修改记录：
*******************************************************************************/		  
void OLED_showNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
  u8 t,temp;
  u8 enshow=0;						   
  for(t=0;t<len;t++)
  {
    temp=(num/OLED_pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
        OLED_showChar(x+(size2/2)*t,y,' ',size2);
        continue;
      }else enshow=1; 
    }
    OLED_showChar(x+(size2/2)*t,y,temp+'0',size2); 
  }
} 

/**********************************************************************************************************
* 函数名    ：   OLED_pow
* 功能描述  ：   m^n函数
* 作者      ：      
* 参数说明  ：   
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/ 
u32 OLED_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}
/**********************************************************************************************************
* 函数名    ：   m^n函数
* 功能描述  ：   OLED_showCHinese
* 作者      ：      
* 参数说明  ：   
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/ 
void OLED_showCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_setPos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_writeByte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_setPos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_writeByte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}
/**********************************************************************************************************
* 函数名    ：   OLED_drawBMP
* 功能描述  ：   功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
* 作者      ：      
* 参数说明  ：   
* 返回值说明：   none
* 修改记录  ： 
* 其他      :
**********************************************************************************************************/ 
void OLED_drawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;

	for(y=y0;y<y1;y++)
	{
		OLED_setPos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_writeByte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 
/*******************************************************************************
* 函数名：  OLED_eraseBMP
* 功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7
* 作者：     	
* 参数说明：  
* 返回值说明：none
* 修改记录：
*******************************************************************************/
void OLED_eraseBMP(u8 x0, u8 y0,u8 x1, u8 y1, u8 BMP[])
{ 	
  u8 x,y;

  for(y=y0;y<y1;y++)
  {
    OLED_setPos(x0,y);
    for(x=x0;x<x1;x++)
    {      
      OLED_writeByte(0x00,OLED_DATA);	    	
    }
  }
} 

/***********************************************************************************************************************
***********************************************************************************************************************/
