#include "display.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include <string.h>

#define EMPTY_SEGMENT 0xFF
#define SEGMENT_AMOUNT 4

uint8_t segments[SEGMENT_AMOUNT] = {EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT, EMPTY_SEGMENT};

const uint8_t SEGMENT_SELECT[] = {0xF1, 0xF2, 0xF4, 0xF8};

const uint8_t NUMBER_MAP[] = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                              0x92, 0x82, 0xF8, 0X80, 0X90};

const uint8_t CHARACTER_MAP[] = {0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E, 0xC2,
                                0x89, 0xCF, 0xE1, 0x8A, 0xC7, 0xEA, 0xC8,
                                0xC0, 0x8C, 0x4A, 0xCC, 0x92, 0x87, 0xC1,
                                0xC1, 0xD5, 0x89, 0x91, 0xA4};

uint8_t dpLibInitialised;

#if defined(__VMA209_LIB_DEBUG__)
  char* dpErrorCode = "";
#endif

void shift(uint8_t val);

ISR(TIMER2_OVF_vect) {
  for(int i = 0; i < SEGMENT_AMOUNT; ++i) {
    shift(segments[i]);
    shift(SEGMENT_SELECT[i]);
    PORTD &= ~(1 << PD4);
    PORTD |= (1 << PD4);
  }
  shift(EMPTY_SEGMENT);
  shift(SEGMENT_SELECT[3]);
  PORTD &= ~(1 << PD4);
  PORTD |= (1 << PD4);
}

void shift(uint8_t val) {
  uint8_t bit;
  for (uint8_t i = 0; i < NUMBER_OF_SEGMENTS; i++) {
    bit = !!(val & (1 << (7 - i)));

    if(bit == 1) {
      PORTB |= (1 << PB0);
    }else {
      PORTB &= ~(1 << PB0);
    }

    // Trigger the clock pin so the display updates
    PORTD |= (1 << PD7);
    PORTD &= ~(1 << PD7);
  }
}

uint8_t initDisplay() {
  if(dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library has already been initialised";
    #endif
    
    return DISPLAY_LIB_ERROR;
  }

  //enable display
  DDRB |= 0b00000001;
  DDRD |= 0b10010000;

  //set up timer
  TCCR2A |= ((1 << WGM20) | (1 << WGM21));
  TCCR2B |= ((1 << CS22) | (1 << CS20));
  TIMSK2 |= (1 << TOIE2);

  sei();

  dpLibInitialised = 1;

  return DISPLAY_LIB_SUCCES;
}

uint8_t dpClearSegment(uint8_t segment) {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  if(segment > 3) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "The specified segement was out of range [0, 3]";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  segments[segment] = EMPTY_SEGMENT;
  return DISPLAY_LIB_SUCCES;
}

uint8_t dpClear() {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif
  }
 
  dpClearSegment(0);
  dpClearSegment(1);
  dpClearSegment(2);
  dpClearSegment(3);

  return DISPLAY_LIB_SUCCES;
}

uint8_t dpWriteNumberToSegment(uint8_t value, uint8_t segment) {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  if(segment > 3) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "The specified segement was out of range [0, 3]";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  if(value > 9) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "The specified value was out of range [0, 9]";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  segments[segment] = NUMBER_MAP[value];
  return DISPLAY_LIB_SUCCES;
}

uint8_t dpWriteNumber(uint16_t number) {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  if(number > 9999) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "The specified number was out of range [0, 9999]";
    #endif

    return DISPLAY_LIB_ERROR;
  }
 
  dpWriteNumberToSegment(number / 1000, 0);
  dpWriteNumberToSegment((number / 100) % 10, 1);
  dpWriteNumberToSegment((number / 10) % 10, 2);
  dpWriteNumberToSegment(number % 10, 3); 

  return DISPLAY_LIB_SUCCES;
}

uint8_t dpWriteCharToSegment(char character, uint8_t segment) {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif

    return DISPLAY_LIB_ERROR;
  }
  
  if(segment > 3) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "The specified segement was out of range [0, 3]";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  if(character >= '0' && character <= '9') {
    dpWriteNumberToSegment(character-48, segment);
    return DISPLAY_LIB_SUCCES;
  }

  if(character == ' ') {
    dpClearSegment(segment);
    return DISPLAY_LIB_SUCCES;
  }

  if(character >= 'a' && character <= 'z') { //elementaire vorm van toUpperCase
    character -= 32;
  }

  if(character < 'A' || character > 'Z') {
    return DISPLAY_LIB_ERROR;
  }

  segments[segment] = CHARACTER_MAP[character-65];

  return DISPLAY_LIB_SUCCES;
}

uint8_t dpWriteString(const char* string) {
  if(!dpLibInitialised) {
    #if defined(__VMA209_LIB_DEBUG__)
      dpErrorCode = "Library was not initialised upon calling this funtion";
    #endif

    return DISPLAY_LIB_ERROR;
  }

  char* output = "    "; //4 tekens en een 0 byte

  for(uint8_t i = 0; i < strlen(string) && i < strlen(output); ++i) {
    output[i] = string[i];
  }

  for(uint8_t i = 0; i < strlen(output); ++i) {
    dpWriteCharToSegment(output[i], i);
  }

  return DISPLAY_LIB_SUCCES;
}

#if defined(__VMA209_LIB_DEBUG__)
const char* dpGetLastError() {
  const char* errorCode = dpErrorCode;
  dpErrorCode = "";

  return errorCode;
}
#endif
