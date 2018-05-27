//
// Created by munk on 5/26/18.
//

#ifndef E6AMS_DLLLAYER_H
#define E6AMS_DLLLAYER_H

#include <stdint.h>

void createControlFrame(uint8_t profile, uint8_t button, uint8_t* frame);
uint8_t getDllFrameSize(uint8_t);

#endif //E6AMS_DLLLAYER_H
