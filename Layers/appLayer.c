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