#ifndef UART_H
#define UART_H

#include <stdint-gcc.h>
#include <stdbool.h>

//***************************************************************
// Public Defines                                               *
//***************************************************************
#define UART_SUCCES             0
#define UART_ERROR_UART_NUM     1
#define UART_ERROR_PARITY       2
#define UART_ERROR_STOP_BITS    3
#define UART_ERROR_SYNC_MODE    4
#define UART_ERROR_CHAR_SIZE    5
#define UART_ERROR_SPEED_MODE   6
#define UART_ERROR_BAUDRATE     7
#define UART_ERROR_RECEIVE      8
#define UART_ERROR_TRANSMIT     9
#define UART_ERROR_TIMEOUT      10

//***************************************************************
// Public Function Definitions                                  *
//***************************************************************
/**
 * Function for setting up UART
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param baudRate  Baudrate for the UART: 110 - 115200
 * @param parity    Is parity used: 'E' for even, 'O' for odd and 'D' for disabled
 * @param stopBits  Number of stopbits used: 1 - 2
 * @param charSize  Size of characters: 5 - 8, 9-bit not supported
 * @param mode      Normal or fast mode: 'N' or 'F'
 * @return          Returns 0 on succes
 */
uint8_t uartInit(uint8_t uartNum, uint32_t baudRate,
              char parity, uint8_t stopBits,
              uint8_t charSize, char mode);

/**
 * Function for sending a single byte
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param value     Character to be sent
 * @return          Returns 0 on succes
 */
uint8_t uartSendByte(uint8_t uartNum, uint8_t value);

/**
 * Function for sending an array of bytes
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param array     Array of bytes to be sent
 * @param size      Size of
 * @return          Returns 0 on succes
 */
uint8_t uartSendByteArray(uint8_t uartNum, uint8_t const * array, uint16_t size);

/**
 * Function for sending a null-terminated string
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param string    Null terminated string to be sendt
 * @return          Returns 0 on succes
 */
uint8_t uartSendString(uint8_t uartNum, char const * string);

/**
 * Function for sending an integer formated as text
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param value     Value to be sent
 * @param base      Base value has to written in: 2 - 36
 * @return          Returns 0 on succes
 */
uint8_t uartSendInteger(uint8_t uartNum, int16_t value, uint8_t base);

/**
 * Function for receiving a single byte.
 * This function is non-blocking and will return error if a byte is not received.
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param value     Pointer to variable to hold received character
 * @return          Returns 0 on succes
 */
uint8_t uartReceiveByte(uint8_t uartNum, uint8_t* value);

/**
 * Function for receiving an array of data
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param value     Pointer to variable to hold received character
 * @param size      Size of receive buffer
 * @return          Returns 0 on succes
 */
uint8_t  uartReceiveByteArray(uint8_t uartNum, uint8_t* value, uint16_t size);

/**
 * Function for receiving a null-terminated string
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @param string    Char buffer for revceived message
 * @return          Returns 0 on succes
 */
uint8_t uartReceiveString(uint8_t uartNum, char* string);

/**
 * Function for checking if a byte has been received
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @return          Returns 0 on if byte is received
 */
uint8_t uartByteReceived(uint8_t uartNum);

/**
 * Function for checking if a byte has been transmitted
 *
 * @param uartNum   Which UART is used: 0 - 3
 * @return          Returns 0 on if byte is transmitted
 */
uint8_t uartByteTransmitted(uint8_t uartNum);

#endif //UART_H
