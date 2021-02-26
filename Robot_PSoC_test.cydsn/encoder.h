/* ========================================
 *
 *  encoder.h
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 *
 * ========================================
*/
#ifndef ENCODER_H_
#define ENCODER_H_
#include "project.h"
#include <stdbool.h>    
double count_to_angv(uint16_t count, uint8_t encoder_counts, bool overflow, bool direction);    
double motor_angv_to_wheel_angv(double motor_angv, uint8_t gear_ratio);
double motor_angv_to_wheel_linv(double motor_angv, uint8_t gear_ratio, double wheel_diameter);
double wheel_linv_to_motor_angv(double wheel_linv, uint8_t gear_ratio, double wheel_diameter);

//The calculations for these functions have been explained in the supplementary information document
double get_robot_linv(double velocity_right, double velocity_left);
double get_robot_angv(double velocity_right, double velocity_left, double robot_width);
double get_robot_v_left(double linv, double angv, double robot_width);
double get_robot_v_right(double linv, double angv, double robot_width);

#endif
/* [] END OF FILE */
