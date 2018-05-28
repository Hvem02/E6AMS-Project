//
// Created by munk on 5/26/18.
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include "dllLayer.h"
#include "appLayer.h"
#include "../Frames/dllFrame.h"
#include "../drivers/uart.h"

#define BUFFER_SIZE 257

static uint8_t writeIndex = 0;
static uint8_t readIndex = 0;
static uint8_t preambleIndex = 0;
static uint16_t fwFrameLength = 0;
static bool frameReady = false;
//static const uint8_t DLLMAXFRAMESIZE = 1 + 2 + 1 + 1 + 2 + 64 + 16;

// Based of the max size of a frame:
//  which is 64 bytes of payload, 16 bytes crc, 2*2 bytes of lengths, 1 byte preamble, version and cmd = 87
//  and the size of a "byte", which is via uart, hence 1 byte start, 8 databits, parity and 1 stop = 11
// at a BAUD of 115200, this is: 87*11*1/115200 = 0.0082s

ISR(TIMER0_OVF_vect) {
    // Do stuff
    frameReady = true;
    // Disable interrupts
    TIMSK0 &= 0b11111000;
}

enum firmwareStates {
    missing_preamble,
    more_data
};

enum firmwareStates firmwareState = missing_preamble;

uint8_t receiveBuffer[BUFFER_SIZE] = {0};


void initDll() {
    // Setup timer0
    TCCR0A &= 0b11111100;
    TCCR0B &= 0b11110111;
    TCCR0B |= 0b00000101; // 1024 prescaler
    TCNT0 = 128;
    TIMSK0 &= 0b11111000;

    // TIFR0 bit 0 bliver sat når der er overflow, nulstilles ved at skrive 1 til det
}


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
    receiveBuffer[writeIndex++] = rcv;
    if (rcv == PREAMBLE) {
        TIMSK0 |= 0b1;
    }
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

            if (receiveBuffer[preambleIndex] == PREAMBLE) {
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

        fwFrameLength = (uint16_t) ((receiveBuffer[preambleIndex+1] << 8) + receiveBuffer[preambleIndex+2]);



        firmwareState = more_data;
    }
}