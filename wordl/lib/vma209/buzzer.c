#include "buzzer.h"

#define __DELAY_BACKWARD_COMPATIBLE__
#include <util/delay.h>
#include <avr/io.h>

uint8_t bzLibInitialised = 0;

#if defined(__VMA209_LIB_DEBUG__)
  char* bzErrorCode = "";
#endif

uint8_t initBuzzer() {
  if(bzLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      bzErrorCode = "Library has already been initialised";
    #endif

    return BUZZER_LIB_ERROR;
  }

  DDRD |= (1 << PD3);
  PORTD |= (1 << PD3);  //disable buzzer when initialising

  bzLibInitialised = 1;

  return BUZZER_LIB_SUCCES;
}

uint8_t bzPlayTone(float tone, uint32_t  msDuration) {
  if(!bzLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      bzErrorCode = "Library was not initialised upon calling function";
    #endif

    return BUZZER_LIB_ERROR;
  }

  uint32_t periodInMicro = (uint32_t) (1000000/tone);
  uint32_t durationInMicro = msDuration * 1000;

  for(uint32_t time = 0; time < durationInMicro; time += periodInMicro) {
    PORTD &= ~(1 << PD3);
    _delay_us(periodInMicro / 2);
    PORTD |= (1 << PD3);
    _delay_us(periodInMicro / 2);
  }

  return BUZZER_LIB_SUCCES;
} 
