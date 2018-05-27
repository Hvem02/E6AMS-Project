//
// Created by munk on 5/26/18.
//

#include "dllLayer.h"
#include "appLayer.h"
#include "../Frames/dllFrame.h"

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