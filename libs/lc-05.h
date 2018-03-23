#ifndef E6AMS_LC_05_H
#define E6AMS_LC_05_H

#include <stdbool.h>
#include <avr/io.h>

bool lc05Test(void);
bool lc05Reset(void);
bool lc05GetVersion(char* buffer, uint16_t size);
bool lc05RestoreDefaultStatus(void);
bool lc05GetAddress(char* buffer, uint16_t size);

#endif //E6AMS_LC_05_H
