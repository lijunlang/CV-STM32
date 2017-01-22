
#ifndef UI_H
#define UI_H

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
#pragma pack(1)
typedef struct 
{
	u16 ui_page;
	u8  ui_power;
}ui_state_t;
#pragma pack()
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
 #define UI_POWER_ON              1
 #define UI_POWER_OFF             0

 #define UI_PAGE_SIZE             5
 #define UI_NO_PAGE               0X0000
 #define UI_MAIN_PAGE             0X0001
 #define UI_FIRST_PAGE            UI_MAIN_PAGE
 #define UI_PULSE_PAGE            0X0002
 #define UI_BP_PAGE               0X0004
 #define UI_STEP_PAGE             0X0008
 #define UI_KAL_DIST_PAGE         0X0010
 #define UI_LAST_PAGE             UI_KAL_DIST_PAGE
 #define UI_ALL_PAGE              0XFFFF
/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
ui_state_t * Ui_stateGet(void);
void Ui_postPage(u32 page);
void Ui_poll(void);
void Ui_init(void);
void Ui_delPage(u32 page);	
void Ui_showAdvertPage(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* UI_H */
