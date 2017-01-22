/*******************************************************************************
Copyright 2008 - 2016 深圳市信盈达科技有限公司. All rights reserved.
文件名:        pedometer.h
描述   :       计步器应用层代码，不涉及算法
作者   :       Jahol Fan
版本   :       V1.0
修改   :   
完成日期：     2016.9.25
*******************************************************************************/
#ifndef PEDOMETER_H
#define PEDOMETER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
//#include "includes.h"
#include "stepAlgorithm.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/
extern OS_TMR   * pedometer_timer;     //计步器软定时器pedometer_timer
/*********************************************************************
 * CONSTANTS
 */

/*********************************************************************
 * MACROS
 */
extern sportsInfo_t userSportsInfo;
/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */

u8 Pedometer_init(void);
sportsInfo_t *Pedometer_getSportsInfo(void);
void Pedometer_startSoftTimer(void);
void Pedometer_stopSoftTimer(void);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* PEDOMETER_H */
