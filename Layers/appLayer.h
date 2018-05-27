//
// Created by munk on 5/27/18.
//

#ifndef E6AMS_APPLAYER_H
#define E6AMS_APPLAYER_H

#include <stdint.h>
#include "../Frames/appFrame.h"

uint16_t appFrameSize(Command command);
void createControlAppFrame(uint8_t profile, uint8_t button, uint8_t* frame);

#endif //E6AMS_APPLAYER_H
