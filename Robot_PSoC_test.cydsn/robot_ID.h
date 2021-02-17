/* ========================================
 *
 * robot_ID.h
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 *
 * ========================================
*/

#ifndef ROBOT_ID_H_
#define ROBOT_ID_H_
#include "project.h"
//Robot ID patch encoding
#define RB1 15 //1111
#define RB2 7 //0111
#define RB3 11 //1011
#define RB4 3 //0011
#define RB5 13 //1101
 
uint8_t get_robot_ID(void);
  
#endif
/* [] END OF FILE */
