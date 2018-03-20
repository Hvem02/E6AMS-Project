//
// Created by munk on 3/16/18.
//

#ifndef E4PRJ4_TRANSPORTER_UART_H
#define E4PRJ4_TRANSPORTER_UART_H

#include <stdint-gcc.h>

/****************************************
* "uart.h":                             *
* Header file for Mega2560 UART driver. *
* Using UART 0.                         *
* Henning Hargaard, 4/11 2015           *
 * Modified by munk on 3/16/18          *
*****************************************/
void InitUART0(unsigned long BaudRate, char Parity,
               unsigned int stop_bits, unsigned int character_size, unsigned char mode);
uint8_t CharReady();
char ReadChar();
void SendChar(char Tegn);
void SendString(char* Streng);
void SendInteger(int Tal);
/****************************************/

#endif //E4PRJ4_TRANSPORTER_UART_H
