/**
 * Button library for Velleman VMA209 Arduino extension board
 * Author: Bauwen De Ron
 * Debug mode: in order to enable advanced debug mode, enable the __VMA209_LIB_DEBUG__ precompiler flag (for more information see: https://docs.platformio.org/en/latest/projectconf/section_env_build.html)
*/

#ifndef __MA209_BUTTON_H
#define _VMA209_BUTTON_H

#include <stdio.h>

#define BUTTON_LIB_ERROR -1
#define BUTTON_LIB_SUCCES 1


uint8_t initButtons();

int8_t btGetState(uint8_t button);

#if defined(__VMA209_LIB_DEBUG__)
  const char* btGetLastError();
#endif

#endif