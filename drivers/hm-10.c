#include "hm-10.h"
#include "uart.h"
#define debugUART 0
#define hm10UART 1
#define hm10BAUDRATE 9600

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";
const char* atTest = "AT";
const char* atEnd = "\r\n";
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
    hm10Ready();
}

bool hm10Ready() {
    char buffer[20] = {0};
    bool success = sendWithResponse(atTest, buffer);
    if (!success) {
        uartSendString(debugUART, "LC05 failed ATTest, response:\r\n");
        uartSendString(debugUART, buffer);
    }
    return success;
}

bool sendWithResponse(const char* message, char* buffer) {

    uartSendString(debugUART, "Sending the following:");
    uartSendString(debugUART, message);
//    uartSendString(debugUART, atEnd);
    uartSendString(hm10UART, message);
    uartSendByte(hm10UART, '\0');
//    uartSendString(hm10UART, atEnd);
    int i = 0;

    while (1) {
        uint8_t response;
//        uint8_t success = readCharWithDelay(hm10UART, &response);
        uint8_t success = uartReceiveByte(hm10UART, &response);

        if (success == UART_ERROR_TIMEOUT) {
            // Timeout error
            uartSendString(debugUART, timeoutError);
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



uint8_t hm10Test(void)
{
    return HM_10_SUCCES;
}

//***************************************************************
// Static Function Implementation                               *
//***************************************************************
