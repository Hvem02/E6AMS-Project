//
// Created by munk on 3/16/18.
//

#ifndef E4PRJ4_TRANSPORTER_UART_H
#define E4PRJ4_TRANSPORTER_UART_H

#include <stdint-gcc.h>
#include <avr/io.h>

/****************************************
* "uart.h":                             *
* Header file for Mega2560 UART driver. *
* Using UART 0.                         *
* Henning Hargaard, 4/11 2015           *
 * Modified by munk on 3/16/18          *
*****************************************/

//struct UART {
//    volatile uint8_t UCSRnA;
//    volatile uint8_t UCSRnB;
//    volatile uint8_t UCSRnC;
//    volatile uint8_t UDRn;
//    volatile uint16_t UBRRn;
//};
//
//struct UART UART0 = {UCSR0A, UCSR0B, UCSR0C, UDR0, UBRR0};
//struct UART UART1 = {UCSR1A, UCSR1B, UCSR1C, UDR1, UBRR1};
//struct UART UART2 = {UCSR2A, UCSR2B, UCSR2C, UDR2, UBRR2};
//struct UART UART3 = {UCSR3A, UCSR3B, UCSR3C, UDR3, UBRR3};

void InitUART0(unsigned long BaudRate, char Parity,
               unsigned int stop_bits, unsigned int character_size, unsigned char mode);
uint8_t CharReady();
char ReadChar();
void SendChar(char Tegn);
void SendString(char* Streng);
void SendInteger(int Tal);
/****************************************/

#endif //E4PRJ4_TRANSPORTER_UART_H
