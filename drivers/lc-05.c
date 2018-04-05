//
// Created by munk on 3/21/18.
//

#include <string.h>
#include "lc-05.h"

void initLC05() {
    // Init uart1
    uartInit(uartLC05, 9600, 'O', 1, 8, 'N');
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
        char res = readCharWithDelay();

        if (res == noReturn) {
            // Timeout error
            uartSendString(0, timeoutError);
            return false;
        } else if (res == 'K' && i > 0) {
            // handle OK
            if (buffer[i-1] == 'O') {
                // response OK !
                return true;
            }
        } else if (res == 'L' && i > 2) {
            if (buffer[i-3] == 'F' && buffer[i-2] == 'A' && buffer[i-1] == 'I') {
                // response Failed !
                return false;
            }
        }

        buffer[i++] = res;
    }
}

void writeErrorCodeToUser(char value) {
    uartSendString(uartDebug, errorCodes[value]);
}