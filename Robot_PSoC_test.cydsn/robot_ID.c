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
    //In order to compare the uint8_t with the macro value, it must be cast to a uint8_t
    //with only the lower bytes preserved (since there are only 4 switches so is only a 4-bit number)
    //Since the switches are active LOW the result must be inverted
    if(dip_switch_encoding == (uint8_t)~(RB1 | 0xF0)){
        return 0x01;
    }else if(dip_switch_encoding == (uint8_t)~(RB2 | 0xF0)){
        return 0x02;    
    }else if(dip_switch_encoding == (uint8_t)~(RB3 | 0xF0)){
        return 0x03;   
    }else if(dip_switch_encoding == (uint8_t)~(RB4 | 0xF0)){
        return 0x04;   
    }else if(dip_switch_encoding == (uint8_t)~(RB5 | 0xF0)){
        return 0x05;   
    } else {
        return 0x05;    //May add more if additional robots compete in the KMC league.  Currently only 5 robots compete per team.
    }
}
    
/* [] END OF FILE */