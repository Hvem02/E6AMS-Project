#include <string.h>
#include <util/delay.h>
#include "hm-10.h"
#include "uart.h"

#define DEBUG_UART          0
#define HM_10_UART          1
#define HM_10_BAUDRATE      9600
#define HM_10_PARITY_BITS   1
#define HM_10_DATABITS      8
#define HM_10_START_BITS    1
#define HM_10_STOP_BITS     1

#define HM_10_ERROR_TIMEOUT 10

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";
const char* atTest = "AT";
const char* responseOK = "OK";
const char* responseFAILED = "FAILED";

char rcvBuffer[255] = {0};
uint8_t rcvIndex = 0;
//***************************************************************
// Macro Functions                                              *
//***************************************************************

//***************************************************************
// Static Function Declaration                                  *
//***************************************************************


//***************************************************************
// Public Function Implementation                               *
//***************************************************************

bool waitingForStatus = false;
bool isSetup = false;

void rcvByteCallback(uint8_t uartNumber) {
    uint8_t rcv;
    uint8_t success = uartReceiveByte(uartNumber, &rcv);

    if (success != UART_SUCCES) {
        uartSendString(DEBUG_UART, "Failed to read when interrupted with error: ");
        uartSendInteger(DEBUG_UART, success, 10);
        uartSendString(DEBUG_UART, "\r\n");
        return;
    }

    rcvBuffer[rcvIndex++] = rcv;

    if (rcvIndex > 1) {
        // At least two bytes are present, hence OK could be possible

        int equalsOK = strcmp(responseOK, rcvBuffer); // If 0, they are the same, less than will be a substring, greater than is false

        if (equalsOK <= 0) {
            // It's an substring, everything is good

        } else {
            // Not an substring
            int equalsFAIL = strcmp(responseFAILED, rcvBuffer);

            if (equalsFAIL == 0) {
                // The command failed
            } else {
                // Another type command, as
            }

        }
    }
}

void hm10Init(void)
{
    uartInit(HM_10_UART, HM_10_BAUDRATE, 'D', HM_10_PARITY_BITS, HM_10_DATABITS, 'N');
    uartSetReceiveByteCallback(HM_10_UART, rcvByteCallback);
    uartSendString(HM_10_UART, atTest);
    for (int i = 0; i < 100; ++i) {
        _delay_ms(16);
    }
    uartSendString(HM_10_UART, atTest);
    for (int i = 0; i < 100; ++i) {
        _delay_ms(16);
    }
    uartSendString(HM_10_UART, atTest);

    while (isSetup == false) {
        for (int i = 0; i < 100; ++i) {
            _delay_ms(16);
        }
    }

    uartSendString(DEBUG_UART, "Done setup hm10!");
}

bool hm10Ready() {
    char buffer[20] = {0};
    bool response = sendWithResponse(atTest, buffer);
//    uartSendString(DEBUG_UART, "Received the following to compare with OK: ");
//    uartSendString(DEBUG_UART, buffer);

    return response;
}

bool sendWithResponse(const char* message, char* buffer) {

    uartSendString(DEBUG_UART, "Sending the following:");
    uartSendString(DEBUG_UART, message);
    buffer[0] = 'a'; // TODO remove this temp hack
    uartSendString(HM_10_UART, message);
    return isSetup;
    /*int i = 0;

    while (1) {
        uint8_t response;
        uint8_t success = readCharWithDelay(HM_10_UART, &response);

        if (success == UART_SUCCES) {
            uartSendString(DEBUG_UART, "Success; ");
            uartSendInteger(DEBUG_UART, response, 10);
        } else if (success == HM_10_ERROR_TIMEOUT) {
            // Timeout error
            buffer[i] = '\0';
            int equals = strcmp(responseOK, buffer); // If 0, they are the same, less than will be a substring, greater than is false

            if (equals <= 0) {
                return true;
            }
            return false;
        }

        buffer[i++] = response;
    }*/
}

uint8_t readCharWithDelay(uint8_t uartNum, uint8_t* retVal)
{
    uint8_t totalBits = (HM_10_DATABITS + HM_10_PARITY_BITS + HM_10_START_BITS + HM_10_STOP_BITS);
    double waitTime = (((double)totalBits / (double)HM_10_BAUDRATE) * (double)1000000);
    for(uint8_t i = 0; i < 10; i++)
    {
        if(uartByteReceived(uartNum) == UART_SUCCES)
        {
            uartReceiveByte(uartNum, retVal);
            return UART_SUCCES;
        }
        _delay_ms(waitTime);
    }
    return HM_10_ERROR_TIMEOUT;
}
//***************************************************************
// Static Function Implementation                               *
//***************************************************************
