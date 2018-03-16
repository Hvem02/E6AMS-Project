//
// Created by munk on 3/16/18.
//

// Teori:

// UCSRnA - Control and status register A:
//  7       6      5       4     3      2       1      0
//| RXCn | TXCn | UDREn | FEn | DORn | UPEn | U2Xn | MPCMn |

// Når RXCn er 1:   Nyt tegn modtaget og kan hentes i UDR
// Når TXCn er 1:   Sender tom, klar til at sende nyt tegn, samt sendeskifteregister tomt
// Når UDREn er 1:  Klar til at sende nyt tegn (der må skrives til UDR
// FEn:             Frame error, modtaget fejl i stopbit
// DORn:            Data overrrun, tegn modtaget inden foregående er blevet læst af SW.
// PEn:             Parity error, modtaget tegn har parity fejl.


// UCSRnB - Control and status register B:
//  7           6       5       4       3        2       1      0
//| RXCIEn | TXIECn | UDRIEn | RXENn | TXENn | UCSZn2 | RXB8n | TXB8n |

// 7-5 (begge inklusiv) bruges ved interrupt styret USART
// RXENn = 1: RX Enable - tænder for modtageren
// TXENn = 1: TX Enable - tænder for senderen

// UCSRnC - Control and status register C
//  7           6         5       4     3         2       1         0
//| UMSELn1 | UMSELn0 | UPMn1 | UPMn0 | USBSn | UCSZn1 | UCSZn0 | UCROLn |


// UCSZn1 og UCSZn0
// USBSn: 0 = 1 stop bit; 1 = 2 stop bits
// UMSELn1 = 0 og UMSELn0 = 0 => Async mode
// UPMn1    | UPMn0 |  Parity Mode
// 0        | 0     |   Disabled
// 0        | 1     |   Reserved
// 1        | 0     |   Enabled, Even Parity
// 1        | 1     |   Enabled, Odd Parity

// UCSZn2   | UCSZn1    | UCSZn0    | Character Size
// 0        |   0       |   0       |   5-bit
// 0        |   0       |   1       |   6-bit
// 0        |   1       |   0       |   7-bit
// 0        |   1       |   1       |   8-bit
// 1        |   1       |   1       |   9-bit
// Rest in between are reserved

// Operating Mode   | Equation for calculating baud rate    | Equation for calculating UBRR Value
// Aync normal mode | BAUD = f_{osc}/(16*UBRRn + 1)         | UBRRn = (f_{osc}/(16*BAUD))-1
// U2Xn = 0         |
// -----------------------------------------------------------------------------------------------
// Async double     | BAUD = f_{osc}/(8*(UBRRn+1))          | UBRRn = (f_{osc}/(8*BAUD))-1
// speed mode       |
// U2Xn = 1

#include "uart.h"
#include <avr/io.h>
#include <stdlib.h>

/**
 * Method for setting up a UART
 *
 * @param BaudRate
 * @param DataBit
 * @param Parity
 */
void InitUART(unsigned long BaudRate, unsigned char DataBit, char Parity) {
    // Enable receiver and transmitter
    UCSR0B |= (0b11 << 3);
}


unsigned char CharReady() {

}


char ReadChar() {
    // læsning af modtaget tegn => char x = UDRx;
}
void SendChar(char Tegn) {
    // Send tegn => char x = "a"; UDRx = x;

}
void SendString(char* Streng) {

}
void SendInteger(int Tal) {

}