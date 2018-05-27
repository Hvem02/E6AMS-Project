#include <string.h>
#include <util/delay.h>
#include "hm-10.h"
#include "uart.h"

#define DEBUG_UART          0
#define HM_10_UART          3
#define HM_10_BAUDRATE      9600
#define HM_10_PARITY_BITS   1
#define HM_10_DATABITS      8
#define HM_10_START_BITS    1
#define HM_10_STOP_BITS     1

#define HM_10_ERROR_TIMEOUT 10
#define BUFFER_SIZE 257
#define SET_LEN 7

const char* timeoutError = "I did not receive a char within the 10 ms timeout\r\n";
const char* atTest = "AT";
const char* responseOK = "OK";
const char* responseFAILED = "FAILED";

char rcvBuffer[BUFFER_SIZE] = {0};

uint8_t rcvIndex = 0;
uint8_t cmdIndex = 0;

//***************************************************************
// Macro Functions                                              *
//***************************************************************

//***************************************************************
// Static Function Declaration                                  *
//***************************************************************


//***************************************************************
// Public Function Implementation                               *
//***************************************************************


enum states {
    pre_init,
    init,
    command,
    fw_upload
};

enum settings {
//    baud,
//    name,
    par,
    noti,
    done
};

enum states currentState;
enum settings currentSettingState;

void rcvByteCallback(uint8_t uartNumber) {
    uint8_t rcv;
    uartReceiveByte(uartNumber, &rcv);
    rcvBuffer[rcvIndex++] = rcv;
}



uint8_t hm10Init(void) {
    uartSendString(DEBUG_UART, "Starting setup\n");
    currentState = pre_init;
    currentSettingState = noti;

    // Init communications
    uint8_t success = uartInit(HM_10_UART, HM_10_BAUDRATE, 'D', HM_10_PARITY_BITS, HM_10_DATABITS, 'N');

    if (success != UART_SUCCES) {
        uartSendString(DEBUG_UART, "Failed to setup uart for HM-10");
        return success;
    }

    success = uartSetReceiveByteCallback(HM_10_UART, rcvByteCallback);

    if (success != UART_SUCCES) {
        uartSendString(DEBUG_UART, "Failed to setup callback for HM-10");
        return success;
    }


    // Check that the HM-10 is ready to receive commands

    while (currentState != command) {

        // TODO Think about removing newData variable and instead exploit the index the buffer uses

        // Handle read of data

        char* index = strstr(&rcvBuffer[cmdIndex], "OK");

        if (index != NULL) {
            // Not null, hence what we asked for is in the buffer
            switch (currentState) {
                case pre_init:
                    uartSendString(DEBUG_UART, "In pre\n");
                    cmdIndex = (uint8_t) ((rcvBuffer-index) + 2);
                    currentState = init;
                    break;
                case init:
                    switch (currentSettingState) {
                        case par:
                            uartSendString(DEBUG_UART, "par\n");
                            cmdIndex = (uint8_t) ((rcvBuffer-index) + SET_LEN);
                            currentSettingState = noti;
                            break;
                        case noti:
                            uartSendString(DEBUG_UART, "noti\n");
                            cmdIndex = (uint8_t) ((rcvBuffer-index) + SET_LEN);
                            currentSettingState = done;
                            break;
                        case done:
                            uartSendString(DEBUG_UART, "done\n");
                            cmdIndex = (uint8_t) ((rcvBuffer-index) + SET_LEN);
                            currentState = command;
                            break;
                        default:
                            uartSendString(DEBUG_UART, "Default\n");
                            break;
                    }
                    break;
                default:
                    uartSendString(DEBUG_UART, "Default2\n");
                    break;
            }
        }

        // Handle send of strings based of the state
        switch (currentState) {
            case pre_init:
                uartSendString(HM_10_UART, atTest);
                break;
            case init:
                switch (currentSettingState) {
                    case par:
                        uartSendString(DEBUG_UART, "Sending par\n");
                        uartSendString(HM_10_UART, "AT+PARI1");
                        break;
                    case noti:
//                        uartSendString(DEBUG_UART, "Sending NOTI\n");
                        uartSendString(HM_10_UART, "AT+NOTI1");
                        break;
                    case done:
                        uartSendString(DEBUG_UART, "Done\n");
                        currentState = command;
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        _delay_ms(16);
        _delay_ms(16);
        _delay_ms(16);
    }

    uartSendString(DEBUG_UART, "Done setup hm10!");

    return 0;
}


void sendCommand(uint8_t buttonVal) {
    uartSendInteger(HM_10_UART, buttonVal, 10);

}

bool hm10Ready() {
    return (currentState == command)? true:false;
}

//***************************************************************
// Static Function Implementation                               *
//***************************************************************
