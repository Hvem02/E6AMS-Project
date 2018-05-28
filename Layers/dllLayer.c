//
// Created by munk on 5/26/18.
//

#include "dllLayer.h"
#include "appLayer.h"
#include "../Frames/dllFrame.h"
#include "../drivers/uart.h"

#define BUFFER_SIZE 257

uint8_t writeIndex = 0;
uint8_t readIndex = 0;
uint8_t preambleIndex = 0;
uint16_t fwFrameLength = 0;

enum firmwareStates {
    missing_preamble,
    more_data
};

enum firmwareStates firmwareState = missing_preamble;

uint8_t rcvBuffer[BUFFER_SIZE] = {0};

uint16_t getDllFrameSize(uint16_t appFrameSize) {
    return getTotalSizeOfDllFrame(appFrameSize);
}

uint16_t getDllSizeByCommand(Command command) {
    uint16_t appSize = appFrameSize(command);
    return getDllFrameSize(appSize);
}

void createControlFrame(uint8_t profile, uint8_t button, uint8_t* frame) {
    uint16_t appSize = appFrameSize(Control);
    uint8_t appFrame[appSize];
    createControlAppFrame(profile, button, appFrame);
    createDllFrame(appFrame, frame, appSize);
}

void receiveDll(uint8_t uartNumber) {
    uint8_t rcv;
    uartReceiveByte(uartNumber, &rcv);
    rcvBuffer[writeIndex++] = rcv;
}

void checkForFW() {
    if (writeIndex == readIndex) {
        // No new data
        return;
    }
    uint8_t bytesReady;

    if (writeIndex > readIndex) {
        bytesReady = writeIndex - readIndex;
    } else {
        bytesReady = (uint8_t ) (BUFFER_SIZE - 2 - readIndex + writeIndex);
    }

    if (firmwareState == missing_preamble) {
        // Make sure we have the Preamble and two length bytes before handling data
        if (bytesReady < 3) {
            return;
        }

        bool preambleFound = false;

        for (uint8_t i = 0; i < bytesReady; ++i) {
            preambleIndex = readIndex + i;

            if (rcvBuffer[preambleIndex] == PREAMBLE) {
                preambleFound = true;
                break;
            }
        }

        if (!preambleFound) {
            return;
        }

        if ((bytesReady-preambleIndex) < 2) {
            return;
        }

        fwFrameLength = (uint16_t) ((rcvBuffer[preambleIndex+1] << 8) + rcvBuffer[preambleIndex+2]);



        firmwareState = more_data;
    }
}