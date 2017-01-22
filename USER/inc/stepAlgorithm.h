/*******************************************************************************
Copyright 2008 - 2016 深圳市信盈达科技有限公司. All rights reserved.
文件名:       stepAlgorithm.h
描述   : 	  计步算法、卡路里消耗
作者   : 	  Jahol Fan
版本   :       V1.0
修改   :   
完成日期：  2016年8月28日
*******************************************************************************/
#ifndef _STEP_ALGORITHM_H
#define _STEP_ALGORITHM_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */ 
#include "rtc.h"
#include "mpu6050.h"
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
typedef struct 
{
	float calories; //卡路里
	float distance;//走路路程，单位为m
	u32 stepCount;	//步数
}sportsInfo_t;
typedef struct 
{
	float height;
	float weight;
}personInfo_t;
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 * FUNCTIONS
 */


sportsInfo_t *onSensorChanged(accValue_t *pAccValue,timeStamp_t *timeStamp_p,personInfo_t * personInfo);
sportsInfo_t *DetectorNewStep(float values,timeStamp_t *timeStamp_p,personInfo_t * personInfo)  ; 
u8 DetectorPeak(float newValue, float oldValue);
float Peak_Valley_Thread(float value) ;
float averageValue(float value[], int n);
u32 * getCount(void);

/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _STEP_ALGORITHM_H */
