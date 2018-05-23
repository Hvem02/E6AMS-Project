// Button | Interrupt | Port
//--------+-----------+------
// UP     | INT2      | PD2
// DOWN   | INT1      | PD1
// LEFT   | INT3      | PD3
// RIGHT  | INT0      | PD0

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint-gcc.h>

#include "buttonInterface.h"

//***************************************************************
// Macros                                                       *
//***************************************************************
#define BUTTON_EVENT_COUNT  2
#define PUSH_NUM            0
#define RELEASE_NUM         1

#define BUTTON_COUNT        4
#define UP_NUM              2
#define DOWN_NUM            1
#define LEFT_NUM            3
#define RIGHT_NUM           0

//***************************************************************
// Static Variable Declaration                                  *
//***************************************************************
static buttonCallback_t buttonCallback[BUTTON_COUNT][BUTTON_EVENT_COUNT] = {{NULL}};
static bool isPressed[BUTTON_COUNT] = {false};

//***************************************************************
// Static Function Declaration                                  *
//***************************************************************
void buttonInterruptHandler(uint8_t buttonNum);

//***************************************************************
// Public Function Implementation                               *
//***************************************************************

void buttonInterfaceInit(void)
{
    DDRD &= 0b11110000;     // Port D [0-3] as input.
    EICRA |= 0b01010101;    // Interrupt on falling and rising edge.
    EIMSK |= 0b00001111;    // External interrupt enable.
    sei();
}

void buttonSetUpPushCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[UP_NUM][PUSH_NUM] = callbackFunc;
}

void buttonSetUpReleaseCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[UP_NUM][RELEASE_NUM] = callbackFunc;
}

void buttonSetDownPushCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[DOWN_NUM][PUSH_NUM] = callbackFunc;
}

void buttonSetDownReleaseCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[DOWN_NUM][RELEASE_NUM] = callbackFunc;
}

void buttonSetLeftPushCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[LEFT_NUM][PUSH_NUM] = callbackFunc;
}

void buttonSetLeftReleaseCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[LEFT_NUM][RELEASE_NUM] = callbackFunc;
}

void buttonSetRightPushCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[RIGHT_NUM][PUSH_NUM] = callbackFunc;
}

void buttonSetRightReleaseCallback(buttonCallback_t callbackFunc)
{
    buttonCallback[RIGHT_NUM][RELEASE_NUM] = callbackFunc;
}

//***************************************************************
// Static Function Implementation                               *
//***************************************************************
void buttonInterruptHandler(uint8_t buttonNum)
{
    if((PIND & (0b00000001u << buttonNum)) != (0b00000001u << buttonNum))
    {
        if(buttonCallback[buttonNum][PUSH_NUM] != NULL && isPressed[buttonNum] == false)
        {
            buttonCallback[buttonNum][PUSH_NUM]();
        }
        isPressed[buttonNum] = true;
    }
    else
    {
        if(buttonCallback[buttonNum][RELEASE_NUM] != NULL && isPressed[buttonNum] == true)
        {
            buttonCallback[buttonNum][RELEASE_NUM]();
        }
        isPressed[buttonNum] = false;
    }
}

//***************************************************************
// Interrupt Vectors                                            *
//***************************************************************
#define BUTTON_ISR_FUNCTION(BUT_NUM)    \
ISR(INT ## BUT_NUM ## _vect)            \
{                                       \
    buttonInterruptHandler(BUT_NUM);    \
}

BUTTON_ISR_FUNCTION(0);
BUTTON_ISR_FUNCTION(1);
BUTTON_ISR_FUNCTION(2);
BUTTON_ISR_FUNCTION(3);
