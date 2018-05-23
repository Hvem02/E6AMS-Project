#ifndef BUTTON_INTERFACE_H
#define BUTTON_INTERFACE_H

//***************************************************************
// Function Pointer Declaration for callbacks                   *
//***************************************************************
typedef void (* buttonCallback_t)(void);

//***************************************************************
// Public Function Definitions                                  *
//***************************************************************
/**
 * Function for initialising the button interface.
 */
void buttonInterfaceInit(void);

/**
 * Function for adding handler function for the UP button being pushed.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc
 */
void buttonSetUpPushCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the UP button being released.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetUpReleaseCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the DOWN button being pushed.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetDownPushCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the DOWN button being released.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetDownReleaseCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the LEFT button being pushed.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetLeftPushCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the LEFT button being released.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetLeftReleaseCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the RIGHT button being pushed.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetRightPushCallback(buttonCallback_t callbackFunc);

/**
 * Function for adding handler function for the RIGHT button being released.
 * To remove the handler function, call this function with a NULL pointer.
 * @param callbackFunc  Function pointer to handler function.
 */
void buttonSetRightReleaseCallback(buttonCallback_t callbackFunc);

#endif //BUTTON_INTERFACE_H
