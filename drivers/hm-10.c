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
    uartInit(HM_10_UART, HM_10_BAUDRATE, 'D', HM_10_PARITY_BITS, HM_10_DATABITS, 'N');
    hm10Ready();
}

bool hm10Ready() {
    char buffer[20] = {0};
    bool success = sendWithResponse(atTest, buffer);
    if (!success) {
        uartSendString(DEBUG_UART, "LC05 failed ATTest, response:\r\n");
        uartSendString(DEBUG_UART, buffer);
    }
    return success;
}

bool sendWithResponse(const char* message, char* buffer) {

    uartSendString(DEBUG_UART, "Sending the following:");
    uartSendString(DEBUG_UART, message);
//    uartSendString(DEBUG_UART, atEnd);
    uartSendString(HM_10_UART, message);
    uartSendByte(HM_10_UART, '\0');
//    uartSendString(HM_10_UART, atEnd);
    int i = 0;

    while (1) {
        uint8_t response;
//        uint8_t success = readCharWithDelay(HM_10_UART, &response);
        uint8_t success = uartReceiveByte(HM_10_UART, &response);

        if (success == HM_10_ERROR_TIMEOUT) {
            // Timeout error
            uartSendString(DEBUG_UART, timeoutError);
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
