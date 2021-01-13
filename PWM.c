/*
 * PWM.c
 *
 * Created: 9/01/2021 8:50:44 PM
 *  Author: chay322
 */ 
#include <avr/io.h>
#include "PWM.h"

/*Setup PWM config settings for timer0 - Non-inverting mode */
void PC_PWM_init_TC0(void){
	DDRB |= (1<<DDB4); //Configure PINB4 / OC0 as output
	
	TCCR0 |= (1<<WGM00);
	TCCR0 &= ~(1<<WGM01); //Phase correct PWM
	
	TCCR0 |= (1<<COM01) | (1<<COM00); //Set OC0 when counting up, clear OC0 when counting down (non-inverting)
	
	TCCR0 |= (1<<CS00);
	TCCR0 &= ~((1<<CS01) | (1<<CS02)); //No prescaled clock input
}

/*Generates PhaseCorrectPWM on PINB4/OC0 using timer0 a 50% duty cycle compare value for OCR0 if input is outside of range 0 - 100
 *DutyCycle is to be a percentage (i.e. a uint8_t from 0 - 100)
 */
void GenPC_PWM_TC0(uint8_t DutyCycle){
	//Timer/Counter0 is an 8 bit counter (counts from 0 - 255)
	//Assuming the counter frequency is 1MHz (1us period) overflow period is 255us
	//DutyCycle is assumed to be a PERCENTAGE (i.e. 0-100)
	//If Dutycycle (on %) is DutyCycle, the OFF % is 100 - DutCycle
	if(DutyCycle >= 0 && DutyCycle <= 100){
		uint8_t compareValue = (uint8_t)((double)(100 - DutyCycle) / 100 * 255);
		OCR0 = compareValue;
	} else {
		OCR0 = 127; //Default 50% duty cycle
	}
}

/*Setup PWM config settings for timer2 - this is for INVERTING mode*/
void PC_PWM_init_TC2(void){
	DDRB |= (1<<DDB7); //PINB7 / OC2 configured as output
	
	TCCR2 |= (1<<WGM20);
	TCCR2 &= ~(1<<WGM21); //PWM phase correct operation
	
	TCCR2 |= (1<<COM21);
	TCCR2 &= ~(1<<COM20); //Clear OC2 when up-counting, Set OC2 when down-counting (inverting)
	
	TCCR2 &= ~((1<<CS22)|(1<<CS21));
	TCCR2 |= (1<<CS20); //No prescaling to the clock	
}
//Generates phase-correct PWM on OC2 / PINB7
//Default is 50% duty cycle if the input is beyond range (i.e. 0 - 100)
void GenPC_PWM_TC2(uint8_t DutyCycle){
	//Timer/Counter2 is an 8-bit counter (0 - 255)
	//Assuming the counter frequency is 1MHz (1us period) overflow period is 255us
	//Dutycycle is assumed to be a PERCENTAGE
	//If Dutycycle (on %) is DutyCycle, the OFF % is 100 - DutyCycle
	if(DutyCycle >= 0 && DutyCycle <= 100){
		uint8_t compareValue = (uint8_t)((double)(100 - DutyCycle) / 100 * 255);
		OCR2 = compareValue;
	} else {
		OCR2 = 127; //Default 50% duty cycle
	}
}

void sync_TC2_TC0(void){
	SFIOR |= (1<<TSM); //Activates Timer synchonisation mode
	SFIOR |= (1<<PSR321) | (1<<PSR0); //Reset the prescalers for all timers
	TCNT0 = 0; //Set both counts to 0
	TCNT2 = 0;
	SFIOR &= ~(1<<TSM); //Clear the synchronisation mode which resets PSR321 and PSR0 so timers count simultaneously
}