/**
 * Led library for Velleman VMA209 Arduino extension board
 * Author: Bauwen De Ron
 * Debug mode: in order to enable advanced debug mode, enable the __VMA209_LIB_DEBUG__ precompiler flag (for more information see: https://docs.platformio.org/en/latest/projectconf/section_env_build.html)
*/

#ifndef _VMA209_LED_H
#define _VMA209_LED_H

#include <stdio.h>

#define LED_LIB_SUCCES 0
#define LED_LIB_ERROR 1


// intitialises the leds on the board and sets up the library
uint8_t initLed();

//enable or disable one led at a time
uint8_t ldEnable(uint8_t led);
uint8_t ldDisable(uint8_t led);

//enable or disable all leds on the board
uint8_t ldEnableAll();
uint8_t ldDisableAll();

//flip the position of a led from on to off or the other wat around
uint8_t ldFlip(uint8_t led);

#if defined (__VMA209_LIB_DEBUG__)
  const char* ldGetLastError();
#endif

#endif
