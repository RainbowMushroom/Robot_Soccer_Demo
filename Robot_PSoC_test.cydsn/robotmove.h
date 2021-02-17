/* ========================================
 * robotmove.h
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 *
 * ========================================
*/
#ifndef ROBOTMOVE_H_
#define ROBOTMOVE_H_
#include "project.h"

void move_right_wheel(double angv, uint16_t motor_max_rpm); //Positive angv -> clockwise, negative angv -> anticlockwise (counterclockwise)
void move_left_wheel(double angv, uint16_t motor_max_rpm);
void move_robot(double linv, double angv, double robot_width, double wheel_diameter, uint16_t motor_max_rpm, uint8_t gear_ratio);

double RPM_to_radps(double RPM);
double radps_to_RPM(double radps);
int8_t radps_to_duty_cycle(double radps, uint16_t motor_max_rpm); //Sign indicates which direction PWM should be applied to motor driver output

/* PID CONTROL */
typedef struct{
    double Pgain, Dgain, Igain;
    double previous_error; //For derivative/difference control
    double error_sum; //For sum/integral control
} PID_DATA;

void PID_initialise(PID_DATA *pid_data, double Pgain, double Dgain, double Igain);
double PID_update(PID_DATA *pid_data, double sensed_velocity, double target_velocity); //Returns a double - representing the speed to be sent to the move_robot function
#endif
/* [] END OF FILE */
