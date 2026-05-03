#include "button.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t btButtons[3] = {0, 0, 0};
uint8_t btLibInitialised = 0;

#if defined(__VMA209_LIB_DEBUG__)
  char* btErrorCode = "";
#endif

ISR(PCINT1_vect) {
  if((PINC & 0b00001000) == 0) {
    _delay_ms(1);
    if((PINC & 0b00001000) == 0) {
      btButtons[2] = 1;  
    }
  }else {
    btButtons[2] = 0;  
  }
  
  if((PINC & 0b00000100) == 0) {
    _delay_ms(1);
    if((PINC & 0b00000100) == 0) {
      btButtons[1] = 1;  
    }
  }else {
    btButtons[1] = 0;  
  }
  
  if((PINC & 0b00000010) == 0) {
    _delay_ms(1);
    if((PINC & 0b00000010) == 0) {
      btButtons[0] = 1;
    }
  }else {
    btButtons[0] = 0;
  }
}

uint8_t initButtons() {
  if(btLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      btErrorCode = "Library has already been initialised";
    #endif

    return BUTTON_LIB_ERROR;  
  }

  DDRC &= 0b11110001;
  PORTC |= 0b00001110;
  PCICR |= 0b00001110;

  PCMSK1 |= 0b00001110;
  
  sei();

  btLibInitialised = 1;
  return BUTTON_LIB_SUCCES;
}

int8_t btGetState(uint8_t button) {
  if(!btLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      btErrorCode = "Button library was not yet initialised upon calling function";
    #endif
    return BUTTON_LIB_ERROR;
  }

  if(button > 2) {
    #if defined(__VMA209_LIB_DEBUG__)
      btErrorCode = "Button number was out of range [0, 2]";
    #endif
    return BUTTON_LIB_ERROR;
  }
   
  return btButtons[button];
}

#if defined (__VMA209_LIB_DEBUG__)
const char* btGetLastError() {
  const char* errorCode = btErrorCode;
  btErrorCode = "";

  return errorCode;
}
#endif
