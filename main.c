//
// Created by munk on 3/15/18.
//

/*
 * Buadrate error in response to speed for 16 MHz
 * following table is found https://arduino.stackexchange.com/a/299
 *
 * U2Xn = 0 => Async Normal mode
 * U2Xn = 1 => Async double speed mode
 *
 * +--------------------+---------------+---------------+
 * | Baud Rate (bps)    | U2Xn = 0      | U2Xn = 1      |
 * |                    | UBRRn | Error | UBRRn | Error |
 * +--------------------+-------+-------+-------+-------+
 * | 2400               | 416   | -0.1% | 832   | 0.0%  |
 * | 4800               | 207   | 0.2%  | 416   | -0.1% |
 * | 9600               | 103   | 0.2%  | 207   | 0.2%  |
 * | 14400              | 68    | 0.6%  | 138   | -0.1% |
 * | 19200              | 51    | 0.2%  | 103   | 0.2%  |
 * | 28800              | 34    | -0.8% | 68    | 0.6%  |
 * | 38400              | 25    | 0.2%  | 51    | 0.2%  |
 * | 57600              | 16    | 2.1%  | 34    | -0.8% |
 * | 76800              | 12    | 0.2%  | 25    | 0.2%  |
 * | 115200             | 8     | -3.5% | 16    | 2.1%  |
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>

#include "drivers/uart.h"
#include "drivers/hm-10.h"
#include "drivers/buttonInterface.h"

#include "Layers/dllLayer.h"
#include "Layers/appLayer.h"

#include "external_libs/md5.h"

ISR(__vector_default){};

uint8_t profile = 1;
const uint8_t profileMin = 1;
const uint8_t profileMax = 5;
bool profileRising = true;


void mainProgram(void);
void testProgramAlex(void);
void testProgramSoren(void);

int main()
{
//    mainProgram();
    testProgramAlex();
//    testProgramSoren();
}

void buttonLeftCallback(void) {

}

void mainProgram(void)
{
    uartInit(0, 115200, 'O', 1, 8, 'N');
    sei();
    hm10Init();
//    buttonInterfaceInit();
//    buttonSetLeftPushCallback(buttonLeftCallback);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1) {

    }
#pragma clang diagnostic pop
}


void sendControl(button_t button, event_t event) {
    uartSendString(0, "Sending\n");
    uint16_t appFrameSize = getControlPayloadSize();
    uint16_t frameSize = getDllFrameSize(appFrameSize);
    uint8_t frame[frameSize];
    createControlFrame(profile, button, frame);
    uartSendByteArray(0, frame, frameSize);
    send(frame, frameSize);
}

void switchProfile(button_t button, event_t event) {
    if (profileRising) {
        if (profile == profileMax) {
            profileRising = false;
            switchProfile(button, event);
            return;
        }
        ++profile;
    } else {
        if (profile == profileMin) {
            profileRising = true;
            switchProfile(button, event);
            return;
        }
        --profile;
    }
}

void testProgramAlex(void)
{
    uartInit(0, 115200, 'O', 1, 8, 'N');
    sei();
    hm10Init();
    buttonInit();
    buttonSetCallback(LEFT, PUSH, sendControl);
    buttonSetCallback(RIGHT, PUSH, sendControl);
    buttonSetCallback(UP, PUSH, switchProfile);


    /*uint8_t dllFrameSizeNoMD5 = 9;
    uint8_t hashLen = 16;
    uint8_t totalSize = dllFrameSizeNoMD5 + hashLen;
    uint8_t frame[totalSize];
    frame[0] = 0xAA;
    frame[1] = 0x00;
    frame[2] = 0x16;
    frame[3] = 0x01;
    frame[4] = 0x01;
    frame[5] = 0x00;
    frame[6] = 0x02;
    frame[7] = 0x01;
    frame[8] = 0x01;

    MD5_CTX md5_ctx;
    MD5Init(&md5_ctx);
    MD5Update(&md5_ctx, frame, dllFrameSizeNoMD5);

    uint8_t hash[hashLen];
    MD5Final(hash, &md5_ctx);

    memcpy(&frame[dllFrameSizeNoMD5], hash, hashLen);*/

//    uartSendString(0, "Total string:\n");

//    uartSendByteArray(0, frame, totalSize);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1)
    {
        _delay_ms(1000);
    }
#pragma clang diagnostic pop
}

uint8_t buffer[100] = {0};
uint8_t counter = 0;

void uartHandler(uint8_t uartNum)
{
    uartSendString(0, "Got the following: ");
    uint8_t ch = 0;
    uartReceiveByte(uartNum, &ch);
    uartSendByte(uartNum, ch);
    uartSendString(0, "Test: ");
    uartSendByte(0, '0');
}

void testProgramSoren(void)
{
    uartInit(0, 115200, 'E', 1, 8, 'N');
    //uartInit(1, 9600, 'D', 1, 8, 'N');

    uartSendString(0, "Test\n\r");

    sei();
    uartSetReceiveByteCallback(0, uartHandler);

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    while(1)
    {
    }
#pragma clang diagnostic pop
}