/* ========================================
 *
 * encoder.c
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/

#include "project.h"
#include "encoder.h"
/* Function: count_to_angv
 * Purpose: Converts a count from the encoder timers into the angular velocity of the encoder shaft
 * Inputs: uint16_t count - the timer count value
 *         encoder_counts - the total number of counts on the encoder in a full 360 degree turn
 * Outputs: double - The MAGNITUDE of the angular velocity
 */
double count_to_angv(uint16_t count, uint8_t encoder_counts){
    //The timer clock frequency is 4MHz, giving each count a period of 250ns
    double time_s = count * 250E-9;
    double encoder_angle = 2 * 3.14159 / encoder_counts; //Angle encoder rotates between each pulse [rad]
    
    return encoder_angle/time_s;
}
/* Function: motor_angv_to_wheel_angv
 * Purpose: Converts the angular velocity of the motor shaft into the angular velocity of the 
 *          wheels based on the gear ratio between the motor shaft and the wheel
 * Inputs: double motor_angv - the angular velocity of the motor [rad/s]
 *         uint8_t gear_ratio - the ratio of wheel_gear_teeth/motor_gear_teeth
 * Outputs: double - The angular velocity of the wheel [rad/s]
 */
double motor_angv_to_wheel_angv(double motor_angv, uint8_t gear_ratio){
    return motor_angv/gear_ratio;
}
/* Function: motor_angv_to_wheel_linv
 * Purpose: Converts the angular velocity of the motor shaft into the linear velocity of the 
 *          wheels based on the gear ratio between the motor shaft and the wheel and the wheel
 *          diameter.
 * Inputs: double motor_angv - the angular velocity of the motor [rad/s]
 *         uint8_t gear_ratio - the ratio of wheel_gear_teeth/motor_gear_teeth
 *         double wheel_diameter - the diameter of the wheel [m]
 * Outputs: double - The linear velocity of the wheel [m/s]
 */
double motor_angv_to_wheel_linv(double motor_angv, uint8_t gear_ratio, double wheel_diameter){
    //Determine the angular velocity of the wheel
    double wheel_angv = motor_angv_to_wheel_angv(motor_angv, gear_ratio);
    //v = r*omega
    return wheel_angv * (wheel_diameter / 2);
}

/* Function: wheel_linv_to_motor_angv
 * Purpose: Converts the linear velocity of the wheel into the angular velocity of the 
 *          motor shaft based on the gear ratio between the motor shaft and the wheel and the wheel
 *          diameter.
 * Inputs: double wheel_linv - the linear velocity of the wheel [m/s]
 *         uint8_t gear_ratio - the ratio of wheel_gear_teeth/motor_gear_teeth
 *         double wheel_diameter - the diameter of the wheel [m]
 * Outputs: double - The angular velocity of the motor shaft [rad/s]
 */
double wheel_linv_to_motor_angv(double wheel_linv, uint8_t gear_ratio, double wheel_diameter){
    //Determine the angular velocity of the wheel using v = r*omega
    double wheel_angv = wheel_linv / (wheel_diameter / 2);
    return wheel_angv * gear_ratio;
}

/* Function: get_robot_linv
 * Purpose: Determines the linear velocity [m/s] of the robot (from the robot mid-point) based on the 
 *          linear velocity of the left and right wheels
 * Inputs: double velocity_right - the linear velocity of the right wheel in m/s
 *         double velocity_left  - the linear velocity of the left wheel in m/s
 *                  [negative velocity: forwards, positive velocity: backwards]
 * Outputs: double - the linear velocity component of the robot (from the robot midpoint) in m/s
 *                      [negative velocity: forwards, positive velocity: backwards]
 */
double get_robot_linv(double velocity_right, double velocity_left){
    return 1/2 * (velocity_left + velocity_right);
}
/* Function: get_robot_angv
 * Purpose: Determines the angular velocity [rad/s] of the robot (about the robot mid-point) based on the 
 *          linear velocity of the left and right wheels
 * Inputs: double velocity_right - the linear velocity of the right wheel in m/s
 *         double velocity_left  - the linear velocity of the left wheel in m/s
 *                  [negative velocity: forwards, positive velocity: backwards]
 * Outputs: double - the angular velocity component of the robot (from the robot midpoint) in rad/s
 *                      [negative angv: anticlockwise/counterclockwise, positive angv: clockwise]
 */
double get_robot_angv(double velocity_right, double velocity_left, double robot_width){
    return 1/robot_width * (velocity_left - velocity_right);
}

/* Function: get_robot_v_left
 * Purpose: Determines the linear velocity [m/s] of the robot's left wheel based on the 
 *          angular and linear velocity components of the robot
 * Inputs: double linv - the linear velocity of the centrepoint of the robot in m/s
 *         double angv  - the angular velocity about the centrepoint of the robot in rad/s
 *         double robot_width - the distance [m] between the left and right wheels
 *                  [negative linv: forwards, positive linv: backwards]
 *                  [negative argv: anticlockwise/counterclockwise, positive argv: clockwise]
 * Outputs: double - the linear velocity of the left wheel in m/s
 *                      [negative velocity: forwards, positive velocity: backwards]
 */
double get_robot_v_left(double linv, double angv, double robot_width){
    return linv + (robot_width)/2 * angv;
}

/* Function: get_robot_v_right
 * Purpose: Determines the linear velocity [m/s] of the robot's right wheel based on the 
 *          angular and linear velocity components of the robot
 * Inputs: double linv - the linear velocity of the centrepoint of the robot in m/s
 *         double angv  - the angular velocity about the centrepoint of the robot in rad/s
 *         double robot_width - the distance [m] between the left and right wheels
 *                  [negative linv: forwards, positive linv: backwards]
 *                  [negative argv: anticlockwise/counterclockwise, positive argv: clockwise]
 * Outputs: double - the linear velocity of the right wheel in m/s
 *                      [negative velocity: forwards, positive velocity: backwards]
 */
double get_robot_v_right(double linv, double angv, double robot_width){
    return linv - (robot_width)/2 * angv;
}
/* [] END OF FILE */