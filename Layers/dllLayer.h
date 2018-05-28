//
// Created by munk on 5/26/18.
//

#ifndef E6AMS_DLLLAYER_H
#define E6AMS_DLLLAYER_H

#include <stdint.h>
#include <stdbool.h>
#include "../Library/command.h"

uint16_t getDllSizeByCommand(Command command);
uint16_t getDllFrameSize(uint16_t appFrameSize);
void createControlFrame(uint8_t profile, uint8_t button, uint8_t* frame);
void receiveDll(uint8_t uartNumber);

#endif //E6AMS_DLLLAYER_H
