
#ifndef _IIC_H
#define _IIC_H

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



/*********************************************************************
 * MACROS
 */

/*********************************************************************
 *PUBLIC FUNCTIONS DECLARE
 */
void IIC_init(void);
void IIC_start(void);
void IIC_stop(void);
u8 IIC_waitAck(void);
void IIC_ack(void);
void IIC_nAck(void);
void IIC_sendByte(u8 txd);
u8 IIC_readByte(unsigned char ack);




/*********************************************************************
*********************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _IIC_H */
