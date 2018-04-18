//
// Created by munk on 3/21/18.
//

#ifndef E6AMS_LC_05_H
#define E6AMS_LC_05_H

#include "uart.h"
#include <stdbool.h>

/**
 * Setup the UART1 for communications with the LC-05
 * Calling lc05Ready for seeing if this is correctly setup.
 * if not it will write a debug message to UART0
 */
void initLC05();

/**
 * Sending the command AT to the lc-05.
 * This should always return OK if UART is correctly setup.
 *
 * @return true if OK else false
 */
bool lc05Ready();

/**
 * Reset (restart) the LC-05
 *
 * @return true if everything went well, else false and debug message on UART0
 */
bool lc05Reset();


void lc05Version(char* versionBuffer);

bool sendWithResponse(const char *message, char* buffer);

void writeErrorCodeToUser(uint8_t value);

uint8_t readCharWithDelay(uint8_t uartNum, uint8_t* retVal);

#endif //E6AMS_LC_05_H
