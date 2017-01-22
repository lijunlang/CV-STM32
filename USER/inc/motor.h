
#ifndef _MOTOR_H
#define _MOTOR_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
#include "timer.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
//马达震动强度
#define MOROT_INTENSITY_0    0    //马达停止震动
#define MOROT_INTENSITY_1    300
#define MOROT_INTENSITY_2    400
#define MOROT_INTENSITY_3    500  //马达全速震动
	
//通过改变TIM2->CCR3的值来改变占空比，从而控制MOTOR的震速
#define MOTOR_PWM_VAL TIM2_PWM_VAL 

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */

void Motor_init(void);
void Motor_ON(u16 intensity,u8 times);


/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _MOTOR_H */
