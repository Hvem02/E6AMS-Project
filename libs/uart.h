//
// Created by munk on 3/16/18.
//

#ifndef E4PRJ4_TRANSPORTER_UART_H
#define E4PRJ4_TRANSPORTER_UART_H

/****************************************
* "uart.h":                             *
* Header file for Mega2560 UART driver. *
* Using UART 0.                         *
* Henning Hargaard, 4/11 2015           *
*****************************************/
void InitUART(unsigned long BaudRate, unsigned char DataBit, char Parity);
unsigned char CharReady();
char ReadChar();
void SendChar(char Tegn);
void SendString(char* Streng);
void SendInteger(int Tal);
/****************************************/

#endif //E4PRJ4_TRANSPORTER_UART_H
