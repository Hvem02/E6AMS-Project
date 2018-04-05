#ifndef UART_H
#define UART_H

#include <stdint-gcc.h>
#include <stdbool.h>

/**
 * Method for setting up UART
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param baudRate  Baudrate for the UART: 110 - 115200
 * @param parity    Is parity used: 'E' for even, 'O' for odd and 'D' for disabled
 * @param stopBits  Number of stopbits used: 1 - 2
 * @param charSize  Size of characters: 5 - 9
 * @param mode      Normal or fast mode: 'N' or 'F'
 */
void initUart(uint8_t uartNum, uint32_t baudRate,
              char parity, uint8_t stopBits,
              uint8_t charSize, char mode);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @return
 */
bool isCharReceived(uint8_t uartNum);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @return
 */
char readChar(uint8_t uartNum);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param character
 */
void sendChar(uint8_t uartNum, char character);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @return
 */
char receiveChar(uint8_t uartNum);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param string
 */
void sendString(uint8_t uartNum, char* string);

/**
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param value
 */
void sendInteger(uint8_t uartNum, int16_t value);

#endif //UART_H
