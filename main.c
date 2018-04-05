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
#include "libs/uart.h"

int main()
{
    uartInit(0, 115200, 'O', 1, 8, 'N');

    while(1)
    {
        uint8_t c[3] = {0};
        uartReceiveByteArray(0, c, 3);
        uartSendByteArray(0, c, 3);

        _delay_ms(10);
    }
}