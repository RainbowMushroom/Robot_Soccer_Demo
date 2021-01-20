/*
 * Prototyping_project.c
 *
 * Created: 8/01/2021 9:34:25 PM
 * Author : chay322
 */ 
#include <stdio.h>
/*AVR header files*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <math.h>
#include "PWM.h"
#include "USART.h"
#include "Encoder.h"

/* *****USER DEFINED MACROS***** */
#define F_CPU 3686400
#define BAUD 115200//This is for the RIT and KMC modes
//#define BAUD 4800 //This is for the MINIROBOT communication mode
//#define MYUBRR F_CPU/(2*BAUD) - 1 //Synchronous master mode
#define MYUBRR F_CPU/16/BAUD-1 //Asynchronous normal mode

#define ENCODER_TIMER_ARRAY_SIZE 10

#define trans_buff_size 16
//Character buffer for fixed point and uint transmission

/* *****Global variables***** */
//volatile unsigned char receivedCommand; //Shared variable - when accessing this in main we must disable RX interrupts temporarily to avoid race conditions
char buffer[trans_buff_size] = {};
//Timer 1 is for timing the slots on the left wheel, timer 3 is for the right wheel
volatile bool timerL_on = false;
volatile bool timerR_on = false;

//Stores multiple historic encoder times to get an averaged speed over multiple pulses within a few ms
volatile uint16_t timerL_count[ENCODER_TIMER_ARRAY_SIZE] = {}; //LEFT WHEEL
volatile uint16_t timerR_count[ENCODER_TIMER_ARRAY_SIZE] = {}; //RIGHT WHEEL

volatile uint8_t timerL_index = 0;
volatile uint8_t timerR_index = 0;

//Encoder hardware parameters
//THESE ARE CURRENTLY DUMMY VALUES FOR SIMULATING PURPOSES ONLY
uint8_t counts_per_rev = 200;//Hardware specific - number of pulses per revolution on the encoder

/* *****INTERRUPT SERVICE ROUTINES***** */
//Receive complete - i.e. there is unread data in the receive buffer
/*ISR(USART0_RX_vect){
	receivedCommand = UDR0;
	//Possibly insert a drive motor command here to send an output signal to the CPLD? based on the received command
}*/

//Encoder timer ISR for the left wheel
//Stores the counter time on timer 1 elapsed between encoder pulses
ISR(INT0_vect){
	if(!timerL_on){
		//Re-start timer 1 to count from 0
		TCNT1 = 0x0000;
		timerL_on = true;
	} else {
		if(timerL_index < ENCODER_TIMER_ARRAY_SIZE){
			//Read the value of timer 1 and place it in buffer
			timerL_count[timerL_index] = TCNT1;
			++timerL_index;
		} else {
			timerL_index = 0x00;
		}
		timerL_on = false;
	}
//	USART0_transmit('1');
}
//Encoder timer ISR for the left wheel
//Tracks the timer overflows to indicate whether the number of counts inserted is accurate
ISR(TIMER3_OVF_vect){
	USART0_TxStr(" [T3] ");
}

//Encoder timer ISR for the right wheel
//Stores the counter time on timer 3 elapsed between encoder pulses
ISR(INT1_vect){
	if(!timerR_on){
		//Re-start timer 3 to count from 0
		TCNT3 = 0x0000;
		timerR_on = true;
	} else {
		//Read the value of timer 3 and place it in buffer
		timerR_count[timerR_index] = TCNT3;
		if(timerR_index < ENCODER_TIMER_ARRAY_SIZE){
			++timerR_index;
		} else {
			timerR_index = 0x00;
		}
		timerR_on = false;
	}
//	USART0_transmit('0');
}

//Encoder timer ISR for the right wheel
//Tracks the timer overflows to indicate whether the number of counts inserted is accurate
ISR(TIMER1_OVF_vect){
	USART0_TxStr(" [T1] ");
}

//Enum declarations for the UART
enum parity_status{NONE = 0, ODD = 1, EVEN = 2};
	
//Encoder functions
//Returns in units [rad/ms]
double get_ang_velocity_left_wheel(void){
	//Gets the average count of the timer1 times and converts to a velocity of mm/ms or m/s
	uint16_t total_count = 0;
	double avg_count = 0;
	for(int i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
		//Temporarily disable timer1 interrupts to eliminate race conditions on the global variable
		//disable_encoder_l();
		EIMSK &= ~(1<<INT0);
		total_count += timerL_count[i];
		//enable_encoder_l();
		EIMSK |= (1<<INT0);
	}
	 avg_count = (double) total_count / ENCODER_TIMER_ARRAY_SIZE;
	 //Each count - provided there is no prescaling on timer 1 each count corresponds to 250ns [0.000250ms] with a 4MHz clock
	 //Angle between each slit is given as 2*pi/counts_per_rev.
	 //Time elapsed is the average count measured * 0.00025ms per count
	 //Angular velocity is angle/time
	 return (2 * 3.14159 / counts_per_rev) / (avg_count * 250E-9);  
}

double get_ang_velocity_right_wheel(void){
	uint16_t total_count = 0;
	double avg_count = 0;
	for(int i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
		//Temporarily disable timer3 interrupts to eliminate race conditions on the global variable
		//disable_encoder_r();
		EIMSK &= ~(1<<INT1);
		total_count += timerR_count[i];
		//enable_encoder_r();
		EIMSK |= (1<<INT1);
	}
	avg_count = (double) total_count / ENCODER_TIMER_ARRAY_SIZE;
	 //Each count - provided there is no prescaling on timer 1 each count corresponds to 250ns [0.000250ms] with a 4MHz clock
	 //Angle between each slit is given as 2*pi/counts_per_rev.
	 //Time elapsed is the average count measured * 0.00025ms per count
	 //Angular velocity is angle/time
	return (2 * 3.14159 / counts_per_rev) / (avg_count * 250E-9);
}

/*Setup Output/Input pins and pullups */
void GPIO_init(void){
	DDRB |= ((1<<DDB4)|(1<<DDB7));; //PINB4, b7 output (OC0,OC2)

	DDRD &= ~((1<<DDD0)|(1<<DDD1)); //PIND0 input (INT0, INT1) for encoder     
}

int main(void){
  //Enable global interrupts
  sei();
  GPIO_init();
  PC_PWM_init_TC0();
  PC_PWM_init_TC2();
  USART0_init(MYUBRR,ODD); //No parity bit set
  encoder_init(); //Rising edge triggers on INT0 and INT1
  uint8_t test = 80;
  
  sync_TC2_TC0();    
  GenPC_PWM_TC0(test);
  GenPC_PWM_TC2(test);
  
  double vel_l, vel_r;
  double* v_l = &vel_l;
  double* v_r = &vel_r;
  //Transmit timerL_count - DEBUGGING PURPOSES ONLY
  

  

 
  while (1){
	    //Update and obtain current average velocity
	    vel_r = get_ang_velocity_right_wheel();
	    vel_l = get_ang_velocity_left_wheel();

	    USART0_TxStr("Left wheel velocity is: ");
	    USART0_TxDouble(v_l);
	    USART0_TxStr("\rRight wheel velocity is: ");
	    USART0_TxDouble(v_r);
	    USART0_transmit('\r');
		//USART0_TxStr("timerL_count is: {");
		
		/*	for(int i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
				sprintf(buffer, "%u, ", timerL_count[i]);
				char *p = buffer;
				while(*p){
					USART0_TxStr(p++);
				}
			}
			USART0_TxStr("}\rAnd timerR_count is: {");
			for(int i = 0; i < ENCODER_TIMER_ARRAY_SIZE; i++){
				sprintf(buffer, "%u, ", timerR_count[i]);
				char *p = buffer;
				while(*p){
					USART0_TxStr(p++);
				}
			}
		USART0_TxStr("}\r");*/
		/*USART0_TxStr("}\rThese correspond to the average velocity for the left wheel: ");
		
		//Obtain velocity
		vel_l = get_ang_velocity_left_wheel();
		sprintf(buffer, "%f [rad/s]\r", vel_l);
		char *pl = buffer;
		while(*pl){
			USART0_TxStr(pl++);
		}
		USART0_TxStr("\r");
		
		USART0_TxStr("And for the right wheel: ");
		//Obtain velocity
		vel_r = get_ang_velocity_right_wheel();
		int int_part = vel_r / 1;
		float frac_part = vel_r - int_part;
		sprintf(buffer, "%d.%04f [rad/s]\r ", int_part,frac_part);
		char *pr = buffer;
		while(*pr){
			USART0_TxStr(pr++);
		}
		
	USART0_TxStr("}\r");
		*/
	}
	return 0;
}

