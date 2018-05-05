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
    uartInit(HM_10_UART, HM_10_BAUDRATE, 'D', HM_10_PARITY_BITS, HM_10_DATABITS, 'N');
    uartSendString(HM_10_UART, atTest);
    uartSendString(HM_10_UART, atTest);
    uartSendString(HM_10_UART, atTest);
    while (hm10Ready() == false);

    uartSendString(DEBUG_UART, "Done setup hm10!");
}

bool hm10Ready() {
    char buffer[20] = {0};
    bool response = sendWithResponse(atTest, buffer);
    uartSendString(DEBUG_UART, "Received the following to compare with OK: ");
    uartSendString(DEBUG_UART, buffer);

    return response;
}

bool sendWithResponse(const char* message, char* buffer) {

    uartSendString(DEBUG_UART, "Sending the following:");
    uartSendString(DEBUG_UART, message);

    uartSendString(HM_10_UART, message);
    int i = 0;

    while (1) {
        uint8_t response;
        uint8_t success = readCharWithDelay(HM_10_UART, &response);

        if (success == UART_SUCCES) {
            uartSendString(DEBUG_UART, "Success; ");
            uartSendInteger(DEBUG_UART, response, 10);
        } else if (success == HM_10_ERROR_TIMEOUT) {
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
