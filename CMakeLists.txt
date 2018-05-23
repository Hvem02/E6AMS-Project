cmake_minimum_required(VERSION 3.6)
project(E6AMS)

set(CMAKE_C_STANDARD 11)

SET(MCU "atmega2560")
SET(F_CPU "16000000")

SET(CMAKE_C_COMPILER avr-gcc)
SET(CMAKE_CXX_COMPILER avr-g++)

SET(CMAKE_C_FLAGS "-mmcu=${MCU} -DF_CPU=${F_CPU} -Os -Wall -Wextra -Werror -Wno-unused-function")
SET(CMAKE_C_LINK_FLAGS "-mmcu=${MCU}")

SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(SOURCE_FILES main.c drivers/uart.c drivers/uart.h drivers/hm-10.c drivers/hm-10.h drivers/buttonInterface.c drivers/buttonInterface.h)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})