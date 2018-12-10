#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "main.h"

///  --Externals--  ///
extern HANDLE keyboardListenerThread;

///  --Functions--  ///

//  Listens to keybaord low level messages
DWORD WINAPI keyboardListener(__in LPVOID lpParameter);

#endif