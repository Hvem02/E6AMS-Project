//
// Created by munk on 3/21/18.
//

#include <string.h>
#include "lc-05.h"

void initLC05() {
    // Init uart1
    initUart(1, 9600, 'O', 1, 8, 'N');
    if (!lc05Ready()) {
        // TODO send error via uart 0
//        sendString()
    }
}

bool lc05Ready() {
    return sendWithAck(atTest);
}

bool lc05Reset() {
    return sendWithAck(atReset);
}

char* lc05Version() {
    // TODO maybe only return the version, currently it will return +VERSION: <Param> OK
    return sendWithResponse(atVersion);
}


bool sendWithAck(const char* message) {
    if (sendWithResponse(message) == atOK) {
        return true;
    }
    // TODO send error message on uart0, with response
    return false;
}

char* sendWithResponse(const char* message) {
    char at_send_string[strlen(message)+2];
    strcat(at_send_string, "\r\n");
    sendString(uartNum, at_send_string);
    char response[1028];
    int i = 0;
    while (1) {
        char res = readCharWithDelay();

        if (res == noReturn) {
            return response;
        }

        // TODO handle overflow (can we receive larger strings than 1028?

        response[i++] = res;
    }
}

void writeErrorCodeToUser(char value) {
    // TODO send via uart0 from errorCodes[value]

    sendString(0, errorCodes[value]);
}