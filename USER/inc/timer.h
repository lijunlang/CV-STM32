
#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************************************************************
 * INCLUDES
 */
#include "stm32f4xx.h"
/*********************************************************************
 * TYPEDEFS
 */
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
//改变TIM2->CCR3的值来改变占空比
#define TIM2_PWM_VAL TIM2->CCR3  

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void TIM2_initPWM(u32 arr,u32 psc);
void TIM2_enablePWM(void);
void TIM2_disablePWM(void);

void TIM3_init(u16 arr,u16 psc);
void TIM3_disable(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* TIMER_H */
