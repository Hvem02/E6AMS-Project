// UDRn     USART Data Register
//          Data to be transmitted and received data is stored in this register


// UCSRnA   Control and Status Register A
//| 7     | 6     | 5     | 4     | 3     | 2     | 1     | 0     |
//| RXCn  | TXCn  | UDREn | FEn   | DORn  | UPEn  | U2Xn  | MPCMn |
//
// RXCn     : USART Receive Complete
// TXCn     : USART Transmit Complete
// UDREn    : USART Data Register Empty
// FEn      : Frame Error
// DORn     : Data OverRun
// UPEn     : USART Parity Error
// U2Xn     : Double the USART Transmission Speed
// MPCMn    : Multi-processor Communication Mode


// UCSRnB   Control and Status Register B
//| 7      | 6      | 5      | 4      | 3      | 2      | 1      | 0      |
//| RXCIEn | TXIECn | UDRIEn | RXENn  | TXENn  | UCSZn2 | RXB8n  | TXB8n  |
//
// RXCIEn   : RX Complete Interrupt Enable
// TXIECn   : TX Complete Interrupt Enable
// UDRIEn   : USART DATA Register Empty Interrupt Enable
// RXENn    : Receiver Enable
// TXENn    : Transmitter Enable
// UCSZn2   : Character Size 2
// RXB8n    : Receiver Data Bit 8
// TXB8n    : Transmitter Data Bit 8


// UCSRnC   Control and Status Register C
//| 7       | 6       | 5       | 4       | 3       | 2        | 1      | 0       |
//| UMSELn1 | UMSELn0 | UPMn1   | UPMn0   | USBSn   | UCSZn1   | UCSZn0 | UCROLn  |
//
// UMSELn1  : USART Mode Select 1
// UMSELn0  : USART Mode Select 0
// UPMn1    : Parity Mode 1
// UPMn0    : Parity Mode 0
// USBSn    : Stop Bit Select
// UCSZn1   : Character Size 1
// UCSZn0   : Character Size 0
// UCROLn   : Clock Polarity


// UBRRnH   USART Buad Rate Registers
// UBRRnL
//| 15  | 14  | 13  | 12  | 11  | 10  | 9   | 8   |
//| -   | -   | -   | -   | UBRR[11:8]            | UBRRnH
//|                         UBRR[7:0]             | UBRRnL
//| 7   | 6   | 5   | 4   | 3   | 2   | 1   | 0   |
//
// Buadrate error in response to speed for 16 MHz
// following table is found https://arduino.stackexchange.com/a/299
// +-----------------+---------------+---------------+
// | Baud Rate (bps) | Async Normal  | Async Fast    |
// |                 | UBRRn | Error | UBRRn | Error |
// +-----------------+-------+-------+-------+-------+
// |            2400 |   416 | -0.1% |   832 |  0.0% |
// |            4800 |   207 |  0.2% |   416 | -0.1% |
// |            9600 |   103 |  0.2% |   207 |  0.2% |
// |           14400 |    68 |  0.6% |   138 | -0.1% |
// |           19200 |    51 |  0.2% |   103 |  0.2% |
// |           28800 |    34 | -0.8% |    68 |  0.6% |
// |           38400 |    25 |  0.2% |    51 |  0.2% |
// |           57600 |    16 |  2.1% |    34 | -0.8% |
// |           76800 |    12 |  0.2% |    25 |  0.2% |
// |          115200 |     8 | -3.5% |    16 |  2.1% |

#include "uart.h"
#include <avr/io.h>
#include <stdlib.h>

//***************************************************************
// Defines and constants                                        *
//***************************************************************

#define UART_SUCCES             0
#define UART_ERROR_UART_NUM     1
#define UART_ERROR_PARITY       2
#define UART_ERROR_STOP_BITS    3
#define UART_ERROR_SYNC_MODE    4
#define UART_ERROR_CHAR_SIZE    5
#define UART_ERROR_SPEED_MODE   6
#define UART_ERROR_BAUDRATE     7

#define UART_ERROR_UNDEFINED    69


//***************************************************************
// Macro Functions for Genericness								*
//***************************************************************

#define OFFSET(NUM) ((NUM) == 3 ? 0x70 : 0x8 * (NUM))
#define UCSR_A(NUM) (*(&UCSR0A + OFFSET(NUM)))
#define UCSR_B(NUM) (*(&UCSR0B + OFFSET(NUM)))
#define UCSR_C(NUM) (*(&UCSR0C + OFFSET(NUM)))
#define UBRR_(NUM)  (*(&UBRR0 + OFFSET(NUM)))
#define UBRR_L(NUM) (*(&UBRR0L + OFFSET(NUM)))
#define UBRR_H(NUM) (*(&UBRR0H + OFFSET(NUM)))
#define UDR_(NUM)   (*(&UDR0 + OFFSET(NUM)))


//***************************************************************
// Static Function Declaration									*
//***************************************************************
static bool validateUartNumber(uint8_t uartNum);
static bool validateBaudRate(uint32_t baudRate);

static uint8_t enableTransmitterReceiver(uint8_t uartNum);
static uint8_t setParity(uint8_t uartNum, char parity);
static uint8_t setStopBits(uint8_t uartNum, uint8_t stopBits);
static uint8_t setSynchronisationMode(uint8_t uartNum, char syncMode);
static uint8_t setCharacterSize(uint8_t uartNum, uint8_t charSize);
static uint8_t setSpeedMode(uint8_t uartNum, char mode);
static uint8_t setBaudRate(uint8_t uartNum, uint32_t baudRate);

static char getSpeedMode(uint8_t uartNum);

static bool readyToTransmit(uint8_t uartNum);
static bool readyToReceive(uint8_t uartNum);

//***************************************************************
// Public Function Implementation								*
//***************************************************************
uint8_t initUart(uint8_t uartNum, uint32_t baudRate,
                 char parity, uint8_t stopBits,
                 uint8_t charSize, char mode)
{
    if(validateUartNumber(uartNum) == false)
    {
        return UART_ERROR_UART_NUM;
    }

    if(validateBaudRate(baudRate) == false)
    {
        return UART_ERROR_BAUDRATE;
    }

    enableTransmitterReceiver(uartNum);
    setParity(uartNum, parity);
    setStopBits(uartNum, stopBits);
    setSynchronisationMode(uartNum, 'A');
    setCharacterSize(uartNum, charSize);
    setSpeedMode(uartNum, mode);
    setBaudRate(uartNum, baudRate);
}


bool isCharReceived(uint8_t uartNum)
{
    if(validateUartNumber(uartNum) == false)
    {
        return false;
    }

    if((UCSR_A(uartNum) & 0b10000000) == 0b10000000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void sendChar(uint8_t uartNum, char character)
{
    if(validateUartNumber(uartNum) == false)
    {
        return;
    }

    while(readyToTransmit(uartNum) == false)
    {}

    UDR_(uartNum) = (uint8_t)character;
}

char receiveChar(uint8_t uartNum)
{
    if(validateUartNumber(uartNum) == false)
    {
        return '\0';
    }

    while(readyToReceive(uartNum) == false)
    {}

    return UDR_(uartNum);
}

void sendString(uint8_t uartNum, char* string)
{
    if(validateUartNumber(uartNum) == false)
    {
        return;
    }

    while(*string != '\0')
    {
        sendChar(uartNum, *string);
        string++;
    }
}

void sendInteger(uint8_t uartNum, int16_t value)
{
    if(validateUartNumber(uartNum) == false)
    {
        return;
    }

    char buffer[16];
    itoa(value, buffer, 10);
    sendString(uartNum, buffer);
}


//***************************************************************
// Static Function Implementation								*
//***************************************************************
static bool validateUartNumber(uint8_t uartNum)
{
    if(0 <= uartNum && uartNum <= 3)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool validateBaudRate(uint32_t baudRate)
{
    if(100 <= baudRate && baudRate <= 115200)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static uint8_t enableTransmitterReceiver(uint8_t uartNum)
{
    UCSR_B(uartNum) |= 0b00011000;
    return UART_SUCCES;
}

static uint8_t setParity(uint8_t uartNum, char parity)
{
    if (parity == 'E' || parity == 'e')
    {
        UCSR_C(uartNum) |= 0b00100000;   // Even Parity
        UCSR_C(uartNum) &= 0b11101111;
    }
    else if (parity == 'O' || parity == 'o')
    {
        UCSR_C(uartNum) |= 0b00110000;   // Odd Parity
    }
    else if (parity == 'D' || parity == 'd')
    {
        UCSR_C(uartNum) &= 0b11001111;   // Disabled Parity
    }
    else
    {
        return UART_ERROR_PARITY;
    }
}

static uint8_t setStopBits(uint8_t uartNum, uint8_t stopBits)
{
    if (stopBits == 1)
    {
        UCSR_C(uartNum) &= 0b11110111;
    }
    else if (stopBits == 2)
    {
        UCSR_C(uartNum) |= 0b00001000;
    }
    else
    {
        return UART_ERROR_STOP_BITS;
    }
}

static uint8_t setSynchronisationMode(uint8_t uartNum, char syncMode)
{
    if(syncMode == 'A' || syncMode == 'a')
    {
        UCSR_C(uartNum) &= 0b00111111;
    }
    else if(syncMode == 'S' || syncMode == 's')
    {
        UCSR_C(uartNum) &= 0b01111111;
        UCSR_C(uartNum) |= 0b01000000;
    }
    else
    {
        return UART_ERROR_SYNC_MODE;
    }
}

static uint8_t setCharacterSize(uint8_t uartNum, uint8_t charSize)
{
    if (charSize == 5)
    {
        UCSR_B(uartNum) &= 0b11111011;
        UCSR_C(uartNum) &= 0b11111001;
    }
    else if (charSize == 6)
    {
        UCSR_B(uartNum) &= 0b11111011;
        UCSR_C(uartNum) &= 0b11111011;
        UCSR_C(uartNum) |= 0b00000010;
    }
    else if (charSize == 7)
    {
        UCSR_B(uartNum) &= 0b11111011;
        UCSR_C(uartNum) &= 0b11111101;
        UCSR_C(uartNum) |= 0b00000100;
    }
    else if (charSize == 8)
    {
        UCSR_B(uartNum) &= 0b11111011;
        UCSR_C(uartNum) |= 0b00000110;
    }
    else if (charSize == 9)
    {
        UCSR_B(uartNum) |= 0b00000100;
        UCSR_C(uartNum) |= 0b00000110;
    }
    else
    {
        return UART_ERROR_CHAR_SIZE;
    }
}

static uint8_t setSpeedMode(uint8_t uartNum, char mode)
{
    if(mode == 'N' || mode == 'n')
    {
        UCSR_A(uartNum) &= 0b11111101;
    }
    else if(mode == 'F' || mode == 'f')
    {
        UCSR_A(uartNum) |= 0b00000010;
    }
    else
    {
        return UART_ERROR_SPEED_MODE;
    }
}

static uint8_t setBaudRate(uint8_t uartNum, uint32_t baudRate)
{
    if(validateBaudRate(baudRate) == false)
    {
        return;
    }

	char speedMode = getSpeedMode(uartNum);
	uint16_t UBRRValue = 0;

    if(speedMode == 'N')
    {
        // Normal Speed Mode
        UBRRValue = (((8 * baudRate + F_CPU) / (16 * baudRate)) - 1);
    }
    else if(speedMode == 'F')
    {
        // Fast Speed Mode
        UBRRValue = (((4 * baudRate + F_CPU) / (8 * baudRate)) - 1);
    }
    else
    {
        return UART_ERROR_BAUDRATE;
    }

	UBRR_H(uartNum) = (UBRRValue >> 8);
	UBRR_L(uartNum) = UBRRValue;

}

static char getSpeedMode(uint8_t uartNum)
{
	if((UCSR_A(uartNum) & 0b00000010) == 0b00000010)
	{
		return 'F';
	}
	else
	{
		return 'N';
	}
}

static bool readyToTransmit(uint8_t uartNum)
{
    if((UCSR_A(uartNum) & 0b00100000) == 0b00100000)
    {
        return true;
    }
    else
    {
        return false;
    }
}

static bool readyToReceive(uint8_t uartNum)
{
    if((UCSR_A(uartNum) & 0b10000000) == 0b10000000)
    {
        return true;
    }
    else
    {
        return false;
    }
}