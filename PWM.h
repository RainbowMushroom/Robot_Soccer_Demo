/*
 * PWM.h
 *
 * Created: 9/01/2021 8:50:59 PM
 *  Author: chay322
 */

#ifndef PWM_H_
#define PWM_H_
#include <avr/io.h>
/*Setup PWM config settings for timer0 - Non-inverting mode */
void PC_PWM_init_TC0(void); //Non-inverting mode
void GenPC_PWM_TC0(uint8_t DutyCycle);
void PC_PWM_init_TC2(void); //Inverting mode
void GenPC_PWM_TC2(uint8_t DutyCycle);
void sync_TC2_TC0(void); //Synchronises timers 2 and 0

#endif /*PWM_H_*/