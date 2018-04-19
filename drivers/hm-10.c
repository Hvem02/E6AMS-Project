#include <string.h>
#include "hm-10.h"
#include "uart.h"
#define debugUART 0
#define hm10UART 1
#define hm10BAUDRATE 9600

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";
const char* atTest = "AT";
const char* responseOK = "OK";
const char* responseFAILED = "FAILED";
//***************************************************************
// Macro Functions                                              *
//***************************************************************

//***************************************************************
// Static Function Declaration                                  *
//***************************************************************


//***************************************************************
// Public Function Implementation                               *
//***************************************************************
void hm10Init(void)
{
    uartInit(hm10UART, hm10BAUDRATE, 'D', 1, 8, 'N');
    uartSendString(hm10UART, atTest);
    uartSendString(hm10UART, atTest);
    uartSendString(hm10UART, atTest);
    while (hm10Ready() == false);

    uartSendString(debugUART, "Done setup hm10!");
}

bool hm10Ready() {
    char buffer[20] = {0};
    bool response = sendWithResponse(atTest, buffer);
    uartSendString(debugUART, "Received the following to compare with OK: ");
    uartSendString(debugUART, buffer);

    return response;
}

bool sendWithResponse(const char* message, char* buffer) {

    uartSendString(debugUART, "Sending the following:");
    uartSendString(debugUART, message);

    uartSendString(hm10UART, message);
    int i = 0;

    while (1) {
        uint8_t response;
        uint8_t success = readCharWithDelay(hm10UART, &response);
//        uint8_t success = uartReceiveByte(hm10UART, &response);

        if (success == UART_SUCCES) {
            uartSendString(debugUART, "Success; ");
            uartSendInteger(debugUART, response, 10);
        } else if (success == UART_ERROR_TIMEOUT) {
                // Timeout error
//            uartSendString(debugUART, timeoutError);
            buffer[i] = '\0';
            int equals = strcmp(responseOK, buffer); // If 0, they are the same, less than will be a substring, greater than is false

            if (equals <= 0) {
                return true;
            }
            return false;
        }

        buffer[i++] = response;
    }
}

//***************************************************************
// Static Function Implementation                               *
//***************************************************************
