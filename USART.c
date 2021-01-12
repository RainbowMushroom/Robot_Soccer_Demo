/*
 * USART.c
 *
 * Created: 10/01/2021 11:51:29 AM
 *  Author: chay322
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "USART.h"

//Parity:  0 - none, 1 - odd, 2+ - even
//Initialises SYNCHRONOUS operation for the receiver
//Reads on the RISING clock edge
void USART0_init(unsigned int ubrr, uint8_t parity){
	//Disable global interrupts whilst initialising the USART
	cli();
	
	//Set the baud rate according to the baud rate register value calculated
	UBRR0H = (unsigned char)(ubrr >> 8);
	UBRR0L = (unsigned char)ubrr;
	//DDRE &= ~(1<<PE0) //Configures PE0 / RXD0 as an input
	
	//Enable receiver/transmitter, receiver complete interrupts - overrides output to PE0/RXD0
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0C |= (1<<UMSEL0); //Enable synchronous operation
	
	//Parity setting
	if(parity == 0){
		UCSR0C &= ~((1<<UPM01) | (1<<UPM00)); //Disabled (no) parity
	} else if(parity == 1) {
		UCSR0C |= (1<<UPM00) | (1<<UPM01); //Odd parity
	} else {
		UCSR0C |= (1<<UPM01);
		UCSR0C &= ~(1<<UPM00);   //Even parity
	}
	
	//Character size: 8-bits
	UCSR0B &= ~(1<<UCSZ02);
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	
	//Clock rising edge 
	UCSR0C &= ~(1<<UCPOL0);
	
	//1-stop bit
	UCSR0C &= ~(1<<USBS0);
	
	//Re-enable global interrupts
	sei();
}
//This is a very simple blocking implementation which may be better implemented with an interrupt
//It may be necessary to implement error checking in the USART implementation
//Interrupt driven receive is still under construction
unsigned char USART0_receive(void){
	//Wait for the USART Receive complete flag
	while(UCSR0A & (1<<RXC0));
	//Transfer the contents of the buffer register to the output
	return UDR0;
}

//This is, again a very simple blocking implementation which may be replaced with an interrupt
//driven solution
void USART0_transmit(unsigned char data){
	//Wait for the USART data register empty flag (goes high when UDR0 is ready)
	while(!(UCSR0A & (1<<UDRE0)));
}