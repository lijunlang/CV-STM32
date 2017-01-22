
#include "stm32f4xx.h"
#include "delay.h"
#include "oled.h"
#include "spi.h"
#include "iic.h"
#include "mpu6050.h"

static u8 shift_flag = 0;  //用于切换界面显示的标志位
int acc()//裸机程序结构  
{
	u32 n;
	u8 uiBuf[40];
	
	float accX,accY,accZ;                 //以“米/秒^2”为单位的加速度
	short aacx,aacy,aacz;									//加速度传感器原始数据
	delay_init(100);
	IIC_init();
	MPU_Init();
	Spi1_init();
	Oled_init();
  
	while (1)
	{		
    MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //得到加速度传感器数据
		//将三轴数据转换为以“米/秒^2”为单位的加速度
		accX = ((float)(int)aacx/16384) *10;
		accY = ((float)(int)aacy/16384) *10;
		accZ = ((float)(int)aacz/16384) *10; 
		
		
		if(accX>=0)
		{ 
			sprintf((char*)uiBuf,"X:%02d ",(char)accX); // 以整数显示
			OLED_showString(64,1,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"X:-%02d",(char)(-accX)); // 以整数显示
			OLED_showString(64,1,uiBuf,16);				
		}

		
		if(accY>=0)
		{
			sprintf((char*)uiBuf,"Y:%02d ",(char)accY); // 以整数显示
			OLED_showString(64,3,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Y:-%02d",(char)(-accY)); // 以整数显示
			OLED_showString(64,3,uiBuf,16);				
		}
		
		
		if(accZ>=0)
		{
			sprintf((char*)uiBuf,"Z:%02d ",(char)accZ); // 以整数显示
			OLED_showString(64,5,uiBuf,16);				
		}
	  else
		{
			sprintf((char*)uiBuf,"Z:-%02d",(char)(-accZ)); // 以整数显示
			OLED_showString(64,5,uiBuf,16);				
		}		
	
		
	}
			
}



