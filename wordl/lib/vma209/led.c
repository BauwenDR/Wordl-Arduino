#include "led.h"

#include <avr/io.h>

uint8_t ldLibInitialised = 0;

#if defined (__VMA209_LIB_DEBUG__)
  char* ldErrorCode = "";
#endif

uint8_t initLed() {
  if(ldLibInitialised) {

    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library has already been initialised";
    #endif

    return LED_LIB_ERROR;
  }

  DDRB |= 0b00111100;
  ldLibInitialised = 1;

  ldDisableAll();

  return LED_LIB_SUCCES;
}

uint8_t ldEnable(uint8_t led) {
  if(!ldLibInitialised) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library was not intitialised upon calling function";
    #endif

    return LED_LIB_ERROR;
  }

  if(led > 3) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "The specified led was outside of range [0, 2]";
    #endif

    return LED_LIB_ERROR;
  }

  PORTB &= ~(1<<(led+2));

  return LED_LIB_SUCCES;
}

uint8_t ldDisable(uint8_t led) {
  if(!ldLibInitialised) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library was not intitialised upon calling function";
    #endif

    return LED_LIB_ERROR;
  }

  if(led > 3) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "The specified led was outside of range [0, 2]";
    #endif

    return LED_LIB_ERROR;
  }

  PORTB |= (1<<(led+2));

  return LED_LIB_SUCCES;
}

uint8_t ldEnableAll() {
  if(!ldLibInitialised) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library was not intitialised upon calling function";
    #endif

    return LED_LIB_ERROR;
  }

  PORTB &= 0b11000011;
  return LED_LIB_SUCCES;
}

uint8_t ldDisableAll() {
  if(!ldLibInitialised) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library was not intitialised upon calling function";
    #endif

    return LED_LIB_ERROR;
  }

  PORTB |= 0b00111100;
  return LED_LIB_SUCCES;
}

uint8_t ldFlip(uint8_t led) {
  if(!ldLibInitialised) {
    #if defined (__VMA209_LIB_DEBUG__)
      ldErrorCode = "Library was not intitialised upon calling function";
    #endif

    return LED_LIB_ERROR;
  }

  PORTB ^= (1 << (led+2));
  return LED_LIB_SUCCES;

}

#if defined(__VMA209_LIB_DEBUG__)
const char* ldGetLastError() {
  const char* errorCode = ldErrorCode;
  ldErrorCode = "";

  return errorCode;
}
#endif
