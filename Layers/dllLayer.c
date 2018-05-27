//
// Created by munk on 5/26/18.
//

#include "dllLayer.h"
#include "appLayer.h"
#include "../Frames/dllFrame.h"

uint16_t getDllFrameSize(uint16_t appFrameSize) {
    return getSizeOfDllFrame(appFrameSize);
}