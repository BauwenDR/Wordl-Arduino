#include "adc.h"

#include <avr/io.h>

uint8_t adLibInitialised = 0;

#if defined(__VMA209_LIB_DEBUG__)
  char* adErrorCode = "";
#endif

uint8_t initADC() {
  if(adLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      adErrorCode = "Library has already been initialised";
    #endif
  
    return ADC_LIB_ERROR;
  }

  ADMUX |= (1<<REFS0);
  ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
  ADCSRA |= (1<<ADEN);
  
  adLibInitialised = 1;
  return ADC_LIB_SUCCES;
}

int16_t adGetValue() {
  if(!adLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      adErrorCode = "Library was not initialised upon calling function";
    #endif  
    return ADC_LIB_ERROR;
  }

  ADCSRA |= (1<<ADSC);
  loop_until_bit_is_clear(ADCSRA, ADSC);
  return ADC;
}

#if defined(__VMA209_LIB_DEBUG__)
const char* adGetLastError() {
  const char* errorCode = adErrorCode;
  adErrorCode = "";
  return errorCode;
}
#endif
