/*******************************************************************************
Copyright 2008 - 2016 深圳市信盈达科技有限公司. All rights reserved.
文件名:        sht20.c
描述   :       读取温湿度传感器sht20的值
作者   :       Jahol  Fan
版本   :       V1.0
修改   :       none
完成日期：     2016.8.25
*******************************************************************************/

/*******************************************************************************
 * INCLUDES
 */
#include "iic.h"  
#include "delay.h"
#include "sht20.h"
/*******************************************************************************
 * CONSTANTS
 */
#define SHT20ADDR       0x80         //SHT20写地址
#define ACK             0            //应答信号
#define NACK            1            //非应答信号
#define FAIL            1            //读取温湿度失败的返回值。
#define READ_TEMP_COMD  0xf3         //读取温度命令
#define READ_HUMI_COMD  0xf5         //读取湿度命令
/*******************************************************************************
 * TYPEDEFS
 */


/*******************************************************************************
 * LOCAL VARIABLES
 */
 TempHumiValue_t temp_humi_Value;

/*******************************************************************************
 * LOCAL FUNCTIONS
 */
static void SHT20_setResolution(void);
 static void SHT20_setResolution(void);
 void SHT20_softReset(void);
/*******************************************************************************
 * PROFILE CALLBACKS
 */



/*******************************************************************************
 * PUBLIC FUNCTIONS
 */
  /*******************************************************************************
 * 函数名：SHT20_writeOneByte
 * 功能描述：写一个字节
 * 作者：   
 * 参数说明：  写一个字节的数据
 * 返回值说明：返回的是应答信号
 * 修改记录：
*******************************************************************************/
static u8 SHT20_writeOneByte(u8 dataToWrite)
{
	u8 ack; 
	IIC_sendByte(dataToWrite);     //发送字节	
	ack = IIC_waitAck(); 
	return ack;
}
 /*******************************************************************************
 * 函数名：SHT20_softReset
 * 功能描述：SHT20软件复位，主函数中调用
 * 作者：   
 * 参数说明：  
 * 返回值说明：
 * 修改记录：
*******************************************************************************/
static void SHT20_softReset(void)                    
{
	IIC_start();                      //start I2C
	SHT20_writeOneByte(SHT20ADDR&0xfe);  //I2C address + write
	SHT20_writeOneByte(0xfe);                         //soft reset
	IIC_stop();                       //stop I2C
}
 /*******************************************************************************
 * 函数名：SHT20_Init
 * 功能描述：初始化iic总线
 * 作者：   
 * 参数说明：  
 * 返回值说明：成功返回测量的结果，失败返回 1
 * 修改记录：
*******************************************************************************/
 void SHT20_Init(void)
{
	SHT20_softReset();
}
 /*******************************************************************************
 * 函数名：SHT20_readOneByte
 * 功能描述：读一个字节
 * 作者：   
 * 参数说明：  
 * 返回值说明：
 * 修改记录：
*******************************************************************************/
static u8 SHT20_readOneByte(u8 ack)
{
	u8 temp;
	temp=IIC_readByte(ack);	    
	return temp;
}


/*******************************************************************************
 * 函数名：SHT20_setResolution
 * 功能描述：写寄存器，设置帧率
 * 作者：   
 * 参数说明：  
 * 返回值说明：
 * 修改记录：
*******************************************************************************/
static void SHT20_setResolution(void)                    
{
  IIC_start();                                                                        //Start I2C 
  if(SHT20_writeOneByte(SHT20ADDR&0xfe)==ACK)             //I2C address + write + ACK
  {
   if(SHT20_writeOneByte(0xe6)==ACK)                               //写用户寄存器
    {  
     if(SHT20_writeOneByte(0x83)==ACK);    //设置分辨率   11bit RH% 测量时间：12ms(typ.) & 11bit T℃ 测量时间：9ms(typ.) 
    }
  }
   IIC_stop();                                  //Stop I2C         
}
/*******************************************************************************
 * 函数名：SHT20_readTemOrHum
 * 功能描述：读取SHT20的温度或读取湿度测量值  
 * 作者：   
 * 参数说明：  
 * 返回值说明：成功返回测量的结果，失败返回 1
 * 修改记录：
*******************************************************************************/
float SHT20_readTemOrHum(u8 commod)
{
	float temp;                 //温度
	u8 ack=1;
	u8  MSB,LSB;              //温度、相对湿度的寄存器数据
	float Humidity,Temperature; //温湿度的转换结果

	SHT20_setResolution();           //设置帧率8bit,9bit,10 bit,11bit ,12bit,13bit,14bit
	IIC_start();                 //iic开始信号
	ack = SHT20_writeOneByte(SHT20ADDR&0xfe);
	if( ack == ACK )                //写地址，并等待ACK
	{        
		if(SHT20_writeOneByte(commod)==ACK)                      //写命令
		{
		 do
			{
				delay_ms(6);                                      //延时
				IIC_start();                                    //发送开始信号
			}while(SHT20_writeOneByte(SHT20ADDR|0x01)==NACK);      //无应答则整形，还在测量中，如果有应答，则结束当前循环

			MSB = SHT20_readOneByte(ACK);                          //读Data(MSB)，给应答ACK
			LSB = SHT20_readOneByte(ACK);                          //读Data(LSB)，给应答ACK
			SHT20_readOneByte(NACK);                               //读Checksum ，不给应答NACK 
			IIC_stop();                                         //Stop I2C
			LSB &= 0xfc;                                       //Data (LSB) 的后两位在进行物理计算前前须置‘0’ 
			temp = MSB*256 + LSB;                              //十六进制转成十进制

			if (commod==((u8)READ_HUMI_COMD))                         //命令为读取湿度的命令
			{        
				/*-- calculate relative humidity [%RH] --*/ 
				Humidity =(temp*125)/65536-6;                   //公式: RH%= -6 + 125 * SRH/2^16
				return Humidity;                                //返回值：humidity
			}                                                                                                  
			else if(commod==((u8)READ_TEMP_COMD))                     //命令为读取温度的命令
			{        
				 /*-- calculate temperature [°C] --*/
				Temperature = (temp*175.72f)/65536-46.85f;       //公式:T= -46.85 + 175.72 * ST/2^16
				return Temperature;                             //返回值：temperature
			}
		 }
	}
  return FAIL;
}
/*******************************************************************************
 * 函数名：SHT20_readTemAndHum
 * 功能描述：读取SHT20的温度和湿度测量值  
 * 作者：   
 * 参数说明：  
 * 返回值说明：成功返回测量的结果，失败返回 1
 * 修改记录：
*******************************************************************************/

TempHumiValue_t * SHT20_readTemAndHum(void)
{
	temp_humi_Value.humidity = SHT20_readTemOrHum(READ_HUMI_COMD);//读取湿度
	temp_humi_Value.temperature = SHT20_readTemOrHum(READ_TEMP_COMD);//读取湿度
	return &temp_humi_Value;
}



/*******************************************************************************
*******************************************************************************/
