#ifndef HM_10_H
#define HM_10_H

#include <stdint.h>
#include <stdbool.h>

//***************************************************************
// Public Defines                                               *
//***************************************************************
#define HM_10_SUCCES    0
#define HM_10_ERROR     1

//***************************************************************
// Public Function Definitions                                  *
//***************************************************************
/**
 *
 */
void hm10Init(void);

/**
 *
 * @return
 */
uint8_t hm10Test(void);

bool sendWithResponse(const char* message, char* buffer);

bool hm10Ready();

#endif // MH_10_H