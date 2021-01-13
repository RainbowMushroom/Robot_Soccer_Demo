/*
 * Prototyping_project.c
 *
 * Created: 8/01/2021 9:34:25 PM
 * Author : chay322
 */ 
/*AVR header files*/
#include <avr/io.h>
#include <avr/interrupt.h>

#include "PWM.h"
#include "USART.h"
#define F_CPU 100000 //Default 1MHz internal oscillator - will produce an error which may warrant an external crystal
#define BAUD 115200 //This is for the RIT and KMC modes
//#define BAUD 4800 //This is for the MINIROBOT communication mode
//#define MYUBRR F_CPU/(2*BAUD) - 1 //Synchronous master mode
#define MYUBRR F_CPU/16/BAUD-1 //Asynchronous normal mode
/*Recall the default CKSEL3:0 setting is 0001 which is the internal RC oscillator at 1MHz - with the longest startup time (after reset) */
#define BUFFER_SIZE 32

//Global variables
//volatile unsigned char receivedCommand; //Shared variable - when accessing this in main we must disable RX interrupts temporarily to avoid race conditions

/* INTERRUPT SERVICE ROUTINES */
//Receive complete - i.e. there is unread data in the receive buffer
/*ISR(USART0_RX_vect){
	receivedCommand = UDR0;
	//Possibly insert a drive motor command here to send an output signal to the CPLD? based on the received command
}*/
//Enums
enum parity_status{NONE = 0, EVEN = 2, ODD = 1};
	
/*Setup Output/Input pins and pullups */
void GPIO_init(void){
	DDRB |= (1<<DDB4); //PINB4 output (OC0)
}

int main(void){
  //Enable global interrupts
  sei();
  GPIO_init();
  PC_PWM_init_TC0();
  PC_PWM_init_TC2();
  USART0_init(MYUBRR,NONE); 
  uint8_t test = 80;
  sync_TC2_TC0();    
  GenPC_PWM_TC0(test);
  GenPC_PWM_TC2(test);
  //
  USART0_TxStr("Hello world!");
	while (1){
		//USART0_transmit('t');
	}
}

