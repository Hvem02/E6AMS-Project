//
// Created by munk on 3/21/18.
//

#include <string.h>
#include "lc-05.h"

const char uartDebug = 0;
const char uartLC05 = 1;
const uint16_t uartLC05BAUD = 9600;

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";

// AT commands
const char* atOK = "OK";
const char* atFail = "FAIL";
const char* atTest = "AT";
const char* atReset = "AT+RESET";
const char* atVersion = "AT+VERSION?";
const char* atAddress = "AT+ADDR?";
const char* atName = "AT+NAME?";
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



void initLC05() {
    // Init uart1
    uartInit(uartLC05, uartLC05BAUD, 'O', 1, 8, 'N');
    if (!lc05Ready()) {
        // TODO send error via uart 0
        uartSendString(uartDebug, "!!!!!WARNING!!!!\r\nFailed to setup LC05!!!\r\n");
    }
}

bool lc05Ready() {
    char buffer[20] = {0};
    bool success = sendWithResponse(atTest, buffer);
    if (!success) {
        uartSendString(uartDebug, "LC05 failed ATTest, response:\r\n");
        uartSendString(uartDebug, buffer);
    }
}

bool lc05Reset() {
    char buffer[20] = {0};
    bool success = sendWithResponse(atReset, buffer);
    if (!success) {
        uartSendString(uartDebug, "LC05 failed ATReset, response:\r\n");
        uartSendString(uartDebug, buffer);
    }
}

void lc05Version(char* versionBuffer) {
    // TODO maybe only return the version, currently it will return +VERSION: <Param> OK
    sendWithResponse(atVersion, versionBuffer);
}


bool sendWithResponse(const char* message, char* buffer) {
    char at_send_string[strlen(message)+2];
    strcat(at_send_string, "\r\n");
    uartSendString(uartLC05, at_send_string);
    int i = 0;

    while (1) {
        uint8_t response;
        uint8_t success = readCharWithDelay(uartLC05, &response);

        if (success == UART_ERROR_TIMEOUT) {
            // Timeout error
            uartSendString(0, timeoutError);
            return false;
        } else if (response == 'K' && i > 0) {
            // handle OK
            if (buffer[i-1] == 'O') {
                // response OK !
                return true;
            }
        } else if (response == 'L' && i > 2) {
            if (buffer[i-3] == 'F' && buffer[i-2] == 'A' && buffer[i-1] == 'I') {
                // response Failed !
                return false;
            }
        }

        buffer[i++] = response;
    }
}

void writeErrorCodeToUser(uint8_t value) {
    uartSendString(uartDebug, errorCodes[value]);
}