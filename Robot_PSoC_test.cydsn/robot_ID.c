/* ========================================
 * 
 * robot_ID.c
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/
#include "project.h"
#include "robot_ID.h"
/* Function: get_robot_ID
 * Purpose: Gets the robot ID (from 1 - 5) depending on the DIP switch configuration
 * Inputs: none
 * Outputs: uint8_t - the ID of the robot
 *                      
 */
uint8_t get_robot_ID(void){
    uint8_t dip_switch_encoding = ROBOT_ID_REG_Read();
    
    if(dip_switch_encoding == RB1){
        return 0x01;
    }else if(dip_switch_encoding == RB2){
        return 0x02;    
    }else if(dip_switch_encoding == RB3){
        return 0x03;   
    }else if(dip_switch_encoding == RB4){
        return 0x04;   
    } else {
        return 0x05;   
    }
}
    
/* [] END OF FILE */
