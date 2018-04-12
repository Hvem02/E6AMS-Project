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

void writeErrorCodeToUser(char value);

const char uartDebug = 0;
const char uartLC05 = 1;

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";

// AT commands
const char *atOK = "OK";
const char *atFail = "FAIL";
const char *atTest = "AT";
const char *atReset = "AT+RESET";
const char *atVersion = "AT+VERSION?";
const char *atAddress = "AT+ADDR?";
const char *atName = "AT+NAME?";
//const char* atRemoteName = "AT+RNAME?"
const char *atGetRole = "AT+ROLE?";
const char *atSetRole = "AT+ROLE=";

const char *errorCodes[] = {
        "AT command error",
        "Default result",
        "PSKEY write error",
        "Too long length of device name (more than 32B)",
        "No device name",
        "Bluetooth address: NAP is too long.",
        "Bluetooth address: UAP is too long.",
        "Bluetooth address: LAP is too long.",
        "No PIO numbers mask",
        "No PIO number",
        "No bluetooth devices",
        "Too length of devices",
        "No inquire access code",
        "Too long inquire access code",
        "Invalid inquire access code",
        "The length of password is 0",
        "Too long password (more than 16B",
        "Invalid module role",
        "Invalid BAUD rate",
        "Invalid stop bit",
        "invalid parity bit",
        "Authentication device is not at the pair list",
        "SPP lib hasn't been initialized",
        "SPP lib has been repeated initialization",
        "Invalid inquire mode",
        "Too long inquire time",
        "No bluetooth address",
        "Invalid safe mode",
        "Invalid encryption mode"
};


#endif //E6AMS_LC_05_H
