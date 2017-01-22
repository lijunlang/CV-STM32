
#ifndef __RTC_H
#define __RTC_H

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
typedef struct 
{
  u8 year;
  u8 month;
  u8 date;
  u8 week;
  u8 hour;
  u8 minute;
  u8 second;	
  u8 ampm;
}dateAndTime_t;
typedef	struct
{
	u8 hour;
	u8 minute;
	u8 second;
	u8 twentyMsCount;
}timeStamp_t;
/*********************************************************************
*  EXTERNAL VARIABLES
*/

/*********************************************************************
 * CONSTANTS
 */
#define   MONDAY         1
#define   TUESDAY        2
#define   WEDNESDAY      3
#define   THURSDAY       4
#define   FRIDAY         5
#define   SATURDAY       6
#define   SUNDAY         7
 

/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
u8 RTC_init(void);
u8 RTC_initMode(void);
void RTC_setAlarmA(u8 hour, u8 min, u8 sec, u8 week);
dateAndTime_t *RTC_getDateAndTime(void);
u8 RTC_setDateAndTime(dateAndTime_t *dateAndTime);
dateAndTime_t *RTC_returnDateTimePointer(void);
/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H */
