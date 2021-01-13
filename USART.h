/*
 * USART.h
 *
 * Created: 10/01/2021 11:51:13 AM
 *  Author: chay322
 */ 

#ifndef USART_H_
#define USART_H_
#include <avr/io.h>
#include <stdbool.h>

//Parity - 0 - none, 1 - odd, 2+ - even
void USART0_init(unsigned int ubrr, uint8_t parity);
unsigned char USART0_receive(void);
void USART0_transmit(unsigned char data);
void USART0_TxStr(char *str);

#endif /*USART_H_*/
