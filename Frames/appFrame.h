//
// Created by munk on 5/26/18.
//

#ifndef E6AMS_APPFRAME_H
#define E6AMS_APPFRAME_H
#define SEGMENT_MAX_SIZE 64

#include <stdint.h>
#include "../Library/command.h"


typedef struct {
    Command cmd;
    uint16_t payloadLength;
    uint8_t* payload;
} AppFrame;

/**
 * Method used before calling method createControlPayload.
 * Its intent is to be used in conjunction with creation of an array keeping the AppFramePayload.
 *
 * @return The size needed for an array to keep the control payload.
 */
uint16_t getControlPayloadSize();

/**
 * Method for creating an correct control payload for the AppFrame.
 *
 * @param payload   The resulting payload.
 * @param profile   The profile being used.
 * @param button    The button number pressed.
 */
void createControlPayload(uint8_t* payload, uint8_t profile, uint8_t button);

#endif //E6AMS_APPFRAME_H
