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
 * @param BaudRate          The desired baudrate, should be between 110 and 115200, both inclusive
 * @param Parity            If using parity: E or e for even parity, o or O for odd parity, d or D for disabled,
 *                          rest will not setup anything
 * @param stop_bits         The amount of stop bits, 1 and 2 is supported, any other value will not set anything
 * @param character_size    The amount of characters send: 5-9 supported, any other will not set anything up
 * @param mode              Normal mode = 0, fast mode = 1
 */
void initUART0(unsigned long BaudRate, char Parity, unsigned int stop_bits,
               unsigned int character_size, unsigned char mode) {

    if ((BaudRate >= 110) && (BaudRate <= 115200) && (character_size >= 5) && (character_size <= 9)) {


        // UCSRnB - Control and status register B:
        //  7           6       5       4       3        2       1      0
        //| RXCIEn | TXIECn | UDRIEn | RXENn | TXENn | UCSZn2 | RXB8n | TXB8n |

        // 7-5 (begge inklusiv) bruges ved interrupt styret USART
        // RXENn = 1: RX Enable - tænder for modtageren
        // TXENn = 1: TX Enable - tænder for senderen
        // Enable receiver and transmitter
        UCSR0B |= (0b11 << 3);
        // TXENn = 1: TX Enable - tænder for senderen

        // UCSRnC - Control and status register C
        //  7           6         5       4     3         2       1         0
        //| UMSELn1 | UMSELn0 | UPMn1 | UPMn0 | USBSn | UCSZn1 | UCSZn0 | UCROLn |

        // UPMn1    | UPMn0 |  Parity Mode
        // 0        | 0     |   Disabled
        // 0        | 1     |   Reserved
        // 1        | 0     |   Enabled, Even Parity
        // 1        | 1     |   Enabled, Odd Parity

        if (Parity == 'e' || Parity == 'E') {
            UCSR0C |= 0b1 << 5;
            UCSR0C &= ~(0b1 << 4);
        } else if (Parity == 'o' || Parity == 'O') {
            UCSR0C |= 0b11 << 4;
        } else if (Parity == 'd' || Parity == 'D') {
            UCSR0C &= ~(0b11 << 4);
        } else {
            return;
        }

        // USBSn: 0 = 1 stop bit; 1 = 2 stop bits

        if (stop_bits == 1) {
            UCSR0C &= ~(0b1 << 3);
        } else if (stop_bits == 2) {
            UCSR0C |= 0b1 << 3;
        } else {
            return;
        }
        // UMSELn1 = 0 og UMSELn0 = 0 => Async mode
        UCSR0C &= ~(0b11 << 6);

        // UCSZn2   | UCSZn1    | UCSZn0    | Character Size
        // 0        |   0       |   0       |   5-bit
        // 0        |   0       |   1       |   6-bit
        // 0        |   1       |   0       |   7-bit
        // 0        |   1       |   1       |   8-bit
        // 1        |   1       |   1       |   9-bit
        // Rest in between are reserved

        if (character_size == 5) {
            UCSR0B &= ~(0b1 << 2);
            UCSR0C &= ~(0b11 << 1);
        } else if (character_size == 6) {
            UCSR0B &= ~(0b1 << 2);
            UCSR0C &= ~(0b1 << 2);
            UCSR0C |= 0b1 << 1;
        } else if (character_size == 7) {
            UCSR0B &= ~(0b1 << 2);
            UCSR0C &= ~(0b1 << 1);
            UCSR0C |= 0b1 << 2;
        } else if (character_size == 8) {
            UCSR0B &= ~(0b1 << 2);
            UCSR0C |= 0b11 << 1;
        } else if (character_size == 9) {
            UCSR0B |= 0b1 << 2;
            UCSR0C |= 0b11 << 1;
        } else {
            return;
        }


        // UCSRnA - Control and status register A:
        //  7       6      5       4     3      2       1      0
        //| RXCn | TXCn | UDREn | FEn | DORn | UPEn | U2Xn | MPCMn |
        // Operating Mode   | Equation for calculating baud rate    | Equation for calculating UBRR Value
        // Aync normal mode | BAUD = f_{osc}/(16*UBRRn + 1)         | UBRRn = (f_{osc}/(16*BAUD))-1
        // U2Xn = 0         |
        // -----------------------------------------------------------------------------------------------
        // Async double     | BAUD = f_{osc}/(8*(UBRRn+1))          | UBRRn = (f_{osc}/(8*BAUD))-1
        // speed mode       |
        // U2Xn = 1

        if (mode == 0) { // Normal mode
            UCSR0A &= ~(0b1 << 1);
            UBRR0 = (uint16_t) ((F_CPU/(16*BaudRate)) - 1);
        } else if (mode == 1) { // Double speed
            UCSR0A |= 0b1 << 1;
            UBRR0 = (uint16_t) ((F_CPU/(8*BaudRate)) - 1);
        } else {
            return;
        }
    }
}


uint8_t charReady() {
    // UCSRnA - Control and status register A:
    //  7       6      5       4     3      2       1      0
    //| RXCn | TXCn | UDREn | FEn | DORn | UPEn | U2Xn | MPCMn |

    // Når RXCn er 1:   Nyt tegn modtaget og kan hentes i UDR
    return (uint8_t) ((UCSR0A >> 7) & 0b1);
}


char readChar() {
    while (charReady() == 0);

    // læsning af modtaget tegn => char x = UDRx;
    return UDR0;
}
void sendChar(char Tegn) {
    // Send tegn => char x = "a"; UDRx = x;
    while ((UCSR0A & (0b1 << 5)) == 0);

    UDR0 = Tegn;
}
void sendString(char *Streng) {
    while (*Streng != '\0') {
        sendChar(*Streng++);
    }
}
void sendInteger(int Tal) {
    char buffer[8];
    itoa(Tal, buffer, 10);
    sendString(buffer);
}