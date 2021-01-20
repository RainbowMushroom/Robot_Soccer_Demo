/*
 * Encoder.h
 *
 * Created: 19/01/2021 12:34:21 pm
 *  Author: chay322
 */ 

#ifndef ENCODER_H_
#define ENCODER_H_
#include <avr/io.h>


//Use these variables if doing a measurement at fixed time intervals
//uint16_t encoder_l_count, encoder_r_count;

void encoder_init(); //Rising edge generates interrupt requests.  Initialises both timer1 and timer3
void timer1_init(); //For left wheel
void timer3_init(); //For right wheel
void disable_encoder_l();
void disable_encoder_r();
void enable_encoder_l();
void enable_encoder_r();
//The following functions are only relevant if the encoder count is measured at fixed time intervals
//uint16_t get_encoder_l_count(); //Only relevant when measuring encoder count at fixed time intervals
//uint16_t get_encoder_r_count(); //Only relevant when measuring encoder count at fixed time intervals
//double count_to_velocity();	//Only relevant when measuring encoder count at fixed time intervals
//double get_velocity_left();
//double get_velocity_right();

#endif /*ENCODER_H_*/