//
// Created by munk on 5/27/18.
//

#include <string.h>
#include <stdbool.h>

#include <avr/eeprom.h>
#include <avr/io.h>
#include <avr/wdt.h>

#include "appLayer.h"
#include "dllLayer.h"

#include "../drivers/buttonInterface.h"
#include "../Frames/appFrame.h"

static uint8_t profile = 1;
static const uint8_t profileMin = 1;
static uint8_t profileMax = 5;
static bool profileRising = true;
static fwUploadCallback_t fwUploadCallback = NULL;

uint16_t appFrameSize(Command command) {
    return getPayloadSizeBasedOfCommand(command);
}

void createAckNackAppFrameBytes(uint8_t *appFrame, bool ack) {
    createAppFrame(appFrame, AckNack, (uint8_t *)&ack);
}


void sendAckNackAppFrameBytes(bool ack) {
    uint16_t appFrameSize = getPayloadSizeBasedOfCommand(AckNack);
    uint8_t ackFrame[appFrameSize];
    createAckNackAppFrameBytes(ackFrame, ack);
    dllSend(ackFrame, appFrameSize);
}

void setLEDs() {
    // ones = PORTB1
    // twos = PORTL1
    // fours = PORTL5
    switch (profile) {
        case 1:
            PORTL &= 0b11011101;
            PORTB |= 0b10;
            break;
        case 2:
            PORTB &= 0b11111101;
            PORTL &= 0b01111111;
            PORTL |= 0b10;
            break;
        case 3:
            PORTL &= 0b01111111;
            PORTB |= 0b10;
            PORTL |= 0b10;
            break;
        case 4:
            PORTB &= 0b11111101;
            PORTL &= 0b11111101;
            PORTL |= 0b10000000;
            break;
        case 5:
            PORTL &= 0b11011111;
            PORTB |= 0b10;
            PORTL |= 0b10000000;
            break;
        default:
            break;
    }
}

void switchProfile(button_t button, event_t event) {
    if (profileRising) {
        if (profile == profileMax) {
            profileRising = false;
            switchProfile(button, event);
            return;
        }
        ++profile;
    } else {
        if (profile == profileMin) {
            profileRising = true;
            switchProfile(button, event);
            return;
        }
        --profile;
    }
    setLEDs();
}

void sendControl(button_t button, event_t event) {
    uint16_t frameSize = appFrameSize(Control);
    uint8_t frame[frameSize];
    createControlFrame(frame, profile, button);
    dllSend(frame, frameSize);
}

void resetToBootloader() {
    wdt_enable(WDTO_15MS);
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while (1) {

    }
#pragma clang diagnostic pop
}


void readyForFWUpload() {
    uint8_t appFrameLength = 3;
    uint8_t appFrame[appFrameLength]; // Command, length, 0 payload
    createFWReadyFrame(appFrame);
    dllSend(appFrame, appFrameLength);
}

void setFWUploadHandle(fwUploadCallback_t fwUploadCB) {
    fwUploadCallback = fwUploadCB;
}


void appReceive(uint8_t* appFrame) {
    AppFrame appFrameObj;
    createAppFrameFromBytes(&appFrameObj, appFrame);

    switch (appFrameObj.cmd) {
        case FWReset:
            // Set flag and reset to bootloader
            eeprom_update_byte((uint8_t *) FW_UPLOAD_FLAG, FW_UPLOAD_READY);
            resetToBootloader();
            break;
        case FWSegCount:
            if(fwUploadCallback != NULL) {
                fwUploadCallback(&appFrameObj);
            }
            // Send Ack
            sendAckNackAppFrameBytes(true);
            break;
        case FWSeg:
            if(fwUploadCallback != NULL) {
                fwUploadCallback(&appFrameObj);
            }
            // Save the segment
            sendAckNackAppFrameBytes(true);
            break;
        case MaxProfiles:
            profileMax = appFrameObj.payload[0];
            sendAckNackAppFrameBytes(true);
            break;
        default:
            break;
    }
}
