/*
 * Encoder.c
 *
 * Created: 19/01/2021 12:34:08 pm
 *  Author: chay322
 */ 
#include <avr/io.h>
#include "Encoder.h"

//Interrupts setup on PD0/INT0
//Falling-edge interrupts on INT0, INT1 will be correctly initialised and active provided sei(); is set in the main function
//Timer1 is used to calculate the speed
void timer1_init(){
	//Assuming our maximum time interval possible between pulses from the encoder is no longer than 10ms no prescaling is required
	
	//Initialise timer1 register
	TCNT1 = 0x0000;
	
	TCCR1B &= ~((1<<CS12)|(1<<CS10));
	TCCR1B |= (1<<CS11); //Prescaling by 8 - 2us period at 4Mhz
						//With normal counting this will overflow every 131.072ms
	
	TCCR1A &= ~((1<<WGM11)|(1<<WGM10));
	TCCR1B &= ~((1<<WGM13)|(1<<WGM12)); //Normal counting operation (counts from 0x0000 - 0xFFFF or 0 - 65535) 
	
	TIMSK |= (1<<TOIE1); //Enables the overflow interrupt on timer 1 to time events longer than 131.072 ms	
}

void timer3_init(){
	//Prescaling is not necessary provided pulse interval is less than 16ms
	//Initialise timer3 count
	TCNT3 = 0x0000;
	
	TCCR3B &= ~((1<<CS32)|(1<<CS30));
	TCCR3B |= (1<<CS31); //Prescaled by 8 - 2us period at 4Mhz, overflows at 131.072ms
	
	TCCR3A &= ~((1<<WGM31)|(1<<WGM30));
	TCCR3B &= ~((1<<WGM33)|(1<<WGM32)); //Normal counting operation (counts from 0x0000 - 0xFFFF or 0 - 65535)

	ETIMSK |= (1<<TOIE3); //Enables the overflow interrupt on timer 3 to time events longer than 131.072ms. 
}

//Initialises the INT0 and INT1 interrups to be triggered on the rising edge.
//Also initialises timers 1 and 3.
void encoder_init(){
	//Ensure INT0 and INT1 interrupts are disabled when initialising counts
	EIMSK &= ~((1<<INT0)|(1<<INT1));
	
	//Initialise encoder counts -- only relevant if doing measurements at fixed time intervals
	// encoder_l_count = 0x0000;
	// encoder_r_count = 0x0000;
	//Configure INT0 and INT1 pins as inputs (PD0, PD1)
	DDRD &= ~((1<<DDD0)|(1<<DDD1));
	//Enable interrupt requests on Pins PD0 and PD1 (INT0/INT1)
	EIMSK |= (1<<INT0)|(1<<INT1);
	
	//Rising edge generates interrupt request
	EICRA |= (1<<ISC01)|(1<<ISC00);
	timer3_init();
	timer1_init();
}

void disable_encoder_l(){
	//Disable interrupt requests on PD0
	EIMSK &= ~(1<<INT0);
}
void disable_encoder_r(){
	//Disable interrupt requests on PD1
	EIMSK &= ~(1<<INT1);
}
void enable_encoder_l(){
	//Enable interrupt requets on PD0
	EIMSK |= (1<<INT0);
}
void enable_encoder_r(){
	EIMSK &= ~(1<<INT1);
}

//Timer 3 for right wheel, timer 1 for left wheel
double get_velocity_left();
double get_velocity_right();


/*
uint16_t get_encoder_l_count(){
	return encoder_l_count;
}

uint16_t get_encoder_r_count(){
	return encoder_r_count;
}


double count_to_velocity();

double get_velocity_left();
double get_velocity_right();
*/