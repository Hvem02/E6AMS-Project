#!/bin/bash
avrdude -C avrdude.conf -P/dev/ttyACM0 -cwiring -patmega2560 -b115200 -v -D -Uflash:w:../cmake-build-debug/E6AMS