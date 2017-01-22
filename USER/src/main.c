#include "stm32f4xx.h"
#include "key.h"
#include "led.h"
#include "exti.h"
#include "uart.h"
#include "delay.h"
#include "timer.h"
#include "oled.h"
#include "spi.h"
#include "bmp.h"
#include "iic.h"
#include "sht20.h"
#include "hp6.h"
//#include "acc.h"
#include "mpu6050.h"
#include "rtc.h"
#include "ui.h"
#include "motor.h"

u32 n;
u8 page = 1;
u8 uiBuf[40];
u8 BpBuf[24] = {0};
u8 pulseBuf[24] = {0};
u8 mpuBuf[64];
TempHumiValue_t humi_temp_val ; 	//温湿度	

static u8 shift_flag = 0;  				//用于切换界面显示的标志位
float accX,accY,accZ;                 //以“米/秒^2”为单位的加速度
short aacx,aacy,aacz;									//加速度传感器原始数据
void clock_page(void);
void TH_page(void);
void pulse_page(void);
void bp_page(void);
void step_page(void);
void kal_km_page(void);
void HP6_EN(void);


dateAndTime_t* Ui_getRTC(void)	 
{ 
  return RTC_getDateAndTime();
}

int main()//裸机程序结构  
{
	
	//dateAndTime_t *dateAndTime; 		//时间&日期
	delay_init(100);
	led_init();
	key_init();
	IIC_init();
	SHT20_Init();
	Spi1_init();
	Oled_init();
  //必须把HP6的EN管脚使能。否则，IIC总线不正常，因为上拉电阻的问题。
  HP6_EN();
	
	RTC_init();
	
	init_exti0();
  init_uart1();
	TIM3_init(10000-1,5000-1);//10Khz的计数频率，计10次为1ms,计数10000次为1s 
	//TIM2_initPWM(500-1,50-1);	//1Mhz的计数频率,2Khz的PWM.  
	Spi1_init();
	Oled_init();
	IIC_init();
	SHT20_Init();
	Hp_6_init();       //初始化HP6心率血压 模块
	
	MPU_Init();

	Motor_init(); 
	
	MPU_Set_Accel_Fsr(0);
	
	Motor_ON(MOROT_INTENSITY_3,2);  //开机，马达震动2次

	OLED_drawBMP(0,0,128,8,(u8*)BMP_OPEN);
	delay_ms(2000);
	
	MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //得到加速度传感器数据
	
	
	
	while (1)
	{		
		switch(page)
		{
			case 1:
			  OLED_clear();
				clock_page();
				break;
			case 2:
			  OLED_clear();
				TH_page();
				break;
			case 3:
				OLED_clear();
				pulse_page();

				break;
			case 4:
				OLED_clear();
				bp_page();
				step_page();
				break;
			case 5:
				OLED_clear();
				step_page();
				kal_km_page();
				break;		
			case 6:
				OLED_clear();
				kal_km_page();
				break;	
      default:
        break;				
		}

	}
}

void clock_page(void)
{
  while(1)
	{
	  
		/////////////////时间&日期	////////////////////
			dateAndTime_t *dateAndTime; 		//时间&日期	
	
			

			dateAndTime = Ui_getRTC();
			dateAndTime = Ui_getRTC();
			sprintf((char*)uiBuf,"%02d:%02d",dateAndTime->hour,dateAndTime->minute); 	  
			OLED_showString(24+3,5,uiBuf,24);
			
			sprintf((char*)uiBuf,"20%02d.%02d.%02d",dateAndTime->year,dateAndTime->month,dateAndTime->date); 
			OLED_showString(24,3,uiBuf,20);

			OLED_showCHinese(18,0,12);//
			OLED_showCHinese(36,0,13);//
			OLED_showString(50,0," & TIME",16);
		/////////////////////////////////////


		if(key_scan())
		{
			page = page +1;  //切换到page2
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			IIC_stop();
			break;  //跳出当前页面
			
    }			
	}
}


void TH_page(void)
{

	TempHumiValue_t humi_temp_val ; 	//温湿度	
	u8 uiBuf[40];
  while(1)
	{
			OLED_drawBMP(0,0,64,7,(u8*)TUP1);
			OLED_showCHinese(72,0,9);//温
			OLED_showCHinese(90,0,10);//湿
			OLED_showCHinese(108,0,11);//度
			humi_temp_val = *SHT20_readTemAndHum();//获取温度
			sprintf((char*)uiBuf,"%02dc %02d",(char)humi_temp_val.temperature,(char)humi_temp_val.humidity); // 以整数显示
			OLED_showString(64+8,6,uiBuf,16); 	
			OLED_showString(64+8*7,6,"%",16);
		
		if(key_scan())
		{
			page = page +1; //切换到page3
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			IIC_stop();
			
			break;  //跳出当前页面
    }	
	}
}


void pulse_page(void)
{
	HP_6_OpenRate();   //打开心率测量
	

  while(1)
	{
  if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp_smll);	  //显示心率小图
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp_large);	  //显示心率大图
		delay_ms(300);
  }
  	
		//OLED_drawBMP(0,0,54,7,(u8*)Pulse_bmp);
		OLED_showCHinese(90,0,2);//心
		OLED_showCHinese(108,0,3);//率
		//delay_ms(500);
		
		HP_6_GetRateResult();
		HP_6_GetResultData(pulseBuf);
		
		

		sprintf((char*)uiBuf,"%03d",pulseBuf[7]); // //byte 7是心率结果
		
		OLED_showString(64,4,uiBuf,16); 	
		OLED_showString(64+8*3,4,"bpm",16);
		//delay_ms(1000);
	

		if(key_scan())
		{
			page = page +1; //切换到page4
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			IIC_stop();
			HP_6_CloseRate();
			break;  //跳出当前页面
    }	
	}
}
void bp_page(void)
{
	HP_6_CloseRate();
	delay_ms(100);
	HP_6_OpenBp();   //打开血压测量
	
  while(1)
	{
			OLED_drawBMP(0,0,54,7,(u8*)BP_bmp); //D???
			OLED_showCHinese(90,0,4);//血
			OLED_showCHinese(108,0,5);///压	
			
			delay_ms(500);
			HP_6_GetBpResult();                //获取血压测量状态
			HP_6_GetResultData(BpBuf);
			if(0 == BpBuf[7])                   //byte 7是血压测量的状态:0测量中，1测量完成、2测量失败
			OLED_showString(64,4,"testing",16);  
			else if(1 == BpBuf[7])  
				 {
					OLED_showString(64,4,"success",16);

					//Motor_ON(MOROT_INTENSITY_3,3);

					//MOROT_INTENSITY_0;
					sprintf((char*)uiBuf,"%03d/%03d ", BpBuf[10], BpBuf[11]);//byte 10是血压测量结果的高压,byte 11是血压测量结果的低压	
					HP_6_CloseBp();//2
					OLED_showString(64,6,uiBuf,16); 		 
				 }		 
			else if(2 == BpBuf[7])                  
				OLED_showString(64,4,"fail   ",16);
			//else 
			//do nothing
		//delay_ms(1000);
		
		
		if(key_scan())
		{
			page = page +1; //切换到page5
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			IIC_stop();
			HP_6_CloseBp();
			MPU_Init();
			OLED_clear();
			break;  //跳出当前页面
    }	
	}
}


void step_page(void)
{
	HP_6_CloseBp();//2
  while(1)
	{



		MPU_Get_Accelerometer(&aacx,&aacy,&aacz);			  //得到加速度传感器数据
		//将三轴数据转换为以“米/秒^2”为单位的加速度
		accX = ((float)(int)aacx/16384) *10;
		accY = ((float)(int)aacy/16384) *10;
		accZ = ((float)(int)aacz/16384) *10; 
		if(accX>=0)
		{ 
			sprintf((char*)mpuBuf,"X:%02d ",(char)accX); // 以整数显示
			OLED_showString(72,1,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"X:-%02d",(char)(-accX)); // 以整数显示
			OLED_showString(72,1,mpuBuf,16);				
		}

		
		if(accY>=0)
		{
			sprintf((char*)mpuBuf,"Y:%02d ",(char)accY); // 以整数显示
			OLED_showString(72,3,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"Y:-%02d",(char)(-accY)); // 以整数显示
			OLED_showString(72,3,mpuBuf,16);				
		}
		
		
		if(accZ>=0)
		{
			sprintf((char*)mpuBuf,"Z:%02d ",(char)accZ); // 以整数显示
			OLED_showString(72,5,mpuBuf,16);				
		}
	  else
		{
			sprintf((char*)mpuBuf,"Z:-%02d",(char)(-accZ)); // 以整数显示
			OLED_showString(72,5,mpuBuf,16);				
		}		
	
	if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)step_right_first);	  
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)step_left_first);	  
		delay_ms(300);
  }
	////////////////////////////////
		if(key_scan())
		{
			page = page+1;  //切换到page1
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			
			IIC_stop();
			OLED_clear();
			break;  //跳出当前页面
    }	
	}
}
void kal_km_page(void)
{
	
  while(1)
	{
				
	OLED_clear();

	OLED_showString(64,1,"Pretend",16);
	OLED_showString(64,3," to be ",16);
	OLED_showString(64,5,"steping",16);

	if(0 == shift_flag)
  {
		 shift_flag = 1;
     OLED_drawBMP(0,0,54,7,(u8*)step_right_first);	  
		 delay_ms(300);
  }
  else
  {
		shift_flag = 0;
    OLED_drawBMP(0,0,54,7,(u8*)step_left_first);	  
		delay_ms(300);
  }
	////////////////////////////////

		if(key_scan())
		{
			page = 1; //切换到page5
			Motor_ON(MOROT_INTENSITY_3,1);  //马达震动1次
			IIC_stop();
			break;  //跳出当前页面
    }	
	}
}
void HP6_EN(void)
{
	RCC->AHB1ENR|=1<<1;//使能PORTB时钟 
	GPIOB->MODER &= ~(3 << 30); 	//32bit,清零
	GPIOB->MODER |= ((unsigned int )1 << 30);    	//配置PB15 IO口，为通用输出功能
	
	GPIOB->OTYPER &= ~(1 << 15);	  //配置PB15为推挽输出
	
	GPIOB->OSPEEDR &= ~(3 << 30);
	GPIOB->OSPEEDR |= 2 << 15;  	//配置PB15的输出速度 50MHZ	
  GPIOB->ODR &= ~(1<<15);			  //PB15输出低电平	
}

