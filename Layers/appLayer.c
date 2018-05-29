//
// Created by munk on 5/27/18.
//

#include <string.h>
#include <stdbool.h>
#include <avr/io.h>
#include "appLayer.h"
#include "../drivers/buttonInterface.h"
#include "dllLayer.h"

static uint8_t profile = 1;
static const uint8_t profileMin = 1;
static const uint8_t profileMax = 5;
static bool profileRising = true;


uint16_t appFrameSize(Command command) {
    return getPayloadSizeBasedOfCommand(command);
}

void createNackAppFrameBytes(uint8_t* appFrame) {
    createAppFrame(appFrame, AckNack, 0);
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


void appReceive(uint8_t* appFrame) {
    AppFrame appFrameObj;
    createAppFrameFromBytes(&appFrameObj, appFrame);
    // TODO Handle the receive
}