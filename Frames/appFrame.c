//
// Created by munk on 5/26/18.
//

#include "appFrame.h"

const uint8_t CMD_SIZE = 1;
const uint8_t LEN_SIZE = 2;

uint16_t getControlPayloadSize() {
    return CMD_SIZE+LEN_SIZE+2; // cmd + len + profile + button
}

void createControlPayload(uint8_t* payload, uint8_t profile, uint8_t button) {
    payload[0] = Control;
    payload[1] = 0;
    payload[2] = 2;
    payload[3] = profile;
    payload[4] = button;
}