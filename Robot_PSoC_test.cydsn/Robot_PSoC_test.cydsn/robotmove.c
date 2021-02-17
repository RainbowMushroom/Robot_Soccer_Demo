/* ========================================
 * 
 * robotmove.c
 * Authors: chay322
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * ========================================
*/
#include "project.h"
#include "robotmove.h"
#include "encoder.h"

/* Function: move_right_wheel
 * Purpose: Turns the right wheel of the robot at the specified angular velocity
 * Inputs: double angv - the target angular velocity [rad/s] of the right wheel
 *         uint16_t motor_max_rpm  - the max rpm of the motor powered with a 100% duty cycle
 *                  [negative angv: anticlockwise/counterclockwise, positive angv: clockwise]
 * Outputs: none
 *                      
 */
void move_right_wheel(double angv, uint16_t motor_max_rpm){
    //Obtain the duty cycle to apply to the pin
    int8_t duty_cycle = radps_to_duty_cycle(angv, motor_max_rpm);
    uint8_t count_value = (uint8_t)duty_cycle * 255; //8-bit PWM components have a max count of 255
    //Apply the duty cycle to the output pins depending on the sign of duty_cycle
    if(duty_cycle > 0){
        PWM_R_WriteCompare1(count_value);
        PWM_R_WriteCompare2(0);  //Ground the other PWM pin
    } else {
        PWM_R_WriteCompare1(0); //Ground the other PWM pin
        PWM_R_WriteCompare2(count_value);
    }
}

/* Function: move_left_wheel
 * Purpose: Turns the left wheel of the robot at the specified angular velocity
 * Inputs: double angv - the target angular velocity [rad/s] of the left wheel
 *         uint16_t motor_max_rpm  - the max rpm of the motor powered with a 100% duty cycle
 *                  [negative angv: anticlockwise/counterclockwise, positive angv: clockwise]
 * Outputs: none
 *                      
 */
void move_left_wheel(double angv, uint16_t motor_max_rpm){
    //Obtain the duty cycle to apply to the pin
    int8_t duty_cycle = radps_to_duty_cycle(angv, motor_max_rpm);
    uint8_t count_value = (uint8_t)duty_cycle * 255; //8-bit PWM components have a max count of 255
    //Apply the duty cycle to the output pins depending on the sign of duty_cycle
    if(duty_cycle > 0){
        PWM_L_WriteCompare1(count_value);
        PWM_L_WriteCompare2(0);  //Ground the other PWM pin
    } else {
        PWM_L_WriteCompare1(0); //Ground the other PWM pin
        PWM_L_WriteCompare2(count_value);
    }
}

/* Function: move_robot
 * Purpose: Moves the robot with the desired linear and angular velocity components.  This 
 *          should be considered the input to the robot plant (so is not a PID controller)
 * Inputs: double linv - the target linear velocity component [m/s] of the robot
 *         double angv - the target angular velocity component [rad/s] of the robot
 *         double robot_width - the distance [m] between the two wheels on the robot
           double wheel_diameter - the diameter [m] between the wheels
 *         uint16_t motor_max_rpm  - the max rpm of the motor powered with a 100% duty cycle
 *                  [negative angv: anticlockwise/counterclockwise, positive angv: clockwise]
 *                  [negative linv: backwards, positive linv: forwards]
 *         uint8_t gear_ratio - the ratio wheel_gear_teeth/motor_shaft_teeth which determines
                                how many times the motor turns for the wheel to turn a full 360
 * Outputs: none
 *                      
 */
void move_robot(double linv, double angv, double robot_width, double wheel_diameter, uint16_t motor_max_rpm, uint8_t gear_ratio){
    //Calculate the relevant linear velocities which the wheels must generate
    double v_left = get_robot_v_left(linv, angv, robot_width);
    double v_right = get_robot_v_right(linv, angv, robot_width); 
    
    //Convert to the angular velocity to turn the motor shafts
    double motor_l_angv = wheel_linv_to_motor_angv(v_left, gear_ratio, wheel_diameter); 
    double motor_r_angv = wheel_linv_to_motor_angv(v_right, gear_ratio, wheel_diameter);
    
    //Drive the wheels as required
    move_left_wheel(motor_l_angv, motor_max_rpm);
    move_right_wheel(motor_r_angv, motor_max_rpm);
}

/* Function: RPM_to_radps
 * Purpose: Converts an RPM value to rad/s
 * Inputs: double RPM - the angular RPM value
 *         
 * Outputs: double - the rad/s equivalent of the RPM input
 *                      
 */
double RPM_to_radps(double RPM){
    return RPM * 2 * 3.14159 / 60;
}

/* Function: radps_to_RPM
 * Purpose: Converts a rad/s value to RPM
 * Inputs: double radps - the angular rad/s value
 *         
 * Outputs: double - the RPM equivalent of the rad/s input
 *                      
 */
double radps_to_RPM(double radps){
    return radps * 60 / (2 * 3.14159);
}

/* Function: radps_to_duty_cycle
 * Purpose: Converts a target rad/s value to the necessary duty cycle to drive the motor
 * Inputs: double radps - the target angular rad/s value
 *         uint16_t motor_max_rpm - the maximum angular velocity in RPM the motor can rotate at 100% duty cycle DC input
 *         
 * Outputs: int8_t - the duty cycle to apply.  The sign indicates which direction the duty cycle should be applied
 *                  to the motors.  Positive -> clockwise, Negative -> anticlockwise/counterclockwise
 *                      
 */
int8_t radps_to_duty_cycle(double radps, uint16_t motor_max_rpm){
    //Convert the max rpm of the motor to radps
    double motor_max_radps = RPM_to_radps((double)motor_max_rpm);
    
    int8_t duty_cycle;
    //If the magnitiude of the radps is greater than motor_max_radps set the duty cycle to 100
    //Store only the integer part of the solution as greater accuracy is not necessary
    if((radps >= 0 && radps <= motor_max_radps) || (radps < 0 && radps*(-1) <= motor_max_radps)){
        duty_cycle = radps / motor_max_radps;   
    } else {
        duty_cycle = 100;   
    }
    
    return duty_cycle;
}

/* PID CONTROL */

/* Function: PID_initialise
 * Purpose: Initialises the proportional, derivative/difference and integral/sum gains for PID control
 * Inputs: PID_DATA *pid_data: a pointer to the PID_DATA structure to initialise the gains for
 *         double Pgain: Proportional gain to initialise the controller with
 *                Dgain: Derivative/difference gain to initialise the controller with
 *                Igain: Integral/sum gain to initialise the controller with
 * Outputs: None
 *                      
 */
void PID_initialise(PID_DATA *pid_data, double Pgain, double Dgain, double Igain){
    pid_data->Pgain = Pgain;
    pid_data->Dgain = Dgain;
    pid_data->Igain = Igain;
    
    pid_data->error_sum = 0;
    pid_data->previous_error = 0;    
}
/* Function: PID_Update
 * Purpose: Calculates the actuator value to be provided to the robot_move function based on the sensed
 *          velocity and target velocities desired.  These can be either of linear or angular velocities, 
 *          though the gains for each of these components will be different.
 * Inputs: PID_DATA *pid_data: a pointer to the PID_DATA structure to read from
 *         double sensed_velocity: the sensed angular/linear velocity from the encoder
 *         double target_velocity: the desired velocity of the sensed_velocity.
 *    
 * Outputs: double - the actuator value [either an angular or linear velocity depending on the input]
 *                   to be used in moving the robot.
 *                      
 */
double PID_update(PID_DATA *pid_data, double sensed_velocity, double target_velocity){

    double Pterm, Dterm, Iterm;
    
    //Determine the error between the sensed and target velocities
    double error = target_velocity - sensed_velocity;
    double error_difference = error - pid_data->previous_error;
    
    //Update the sum term
    pid_data->error_sum += error;
    
    Pterm = pid_data->Pgain * error;
    Dterm = pid_data->Dgain * error_difference;
    Iterm = pid_data->Igain * pid_data->error_sum;
    
    //Update the error term in pid_data
    pid_data->previous_error = error;
    
    return Pterm + Dterm + Iterm;
}
/* [] END OF FILE */