//
// Created by munk on 5/27/18.
//

#include "appLayer.h"


uint16_t appFrameSize(Command command) {
    switch (command) {
        case Control:
            return getControlPayloadSize();
        default:
            return 0;
    }
}

void createControlAppFrame(uint8_t profile, uint8_t button, uint8_t* frame) {
    createControlPayload(frame, profile, button);
}