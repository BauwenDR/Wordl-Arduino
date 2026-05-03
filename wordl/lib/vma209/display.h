/**
 * 7 Segment Display library for Velleman VMA209 Arduino extension board
 * Author: Bauwen De Ron + KdG
 * Debug mode: in order to enable advanced debug mode, enable the __VMA209_LIB_DEBUG__ precompiler flag (for more information see: https://docs.platformio.org/en/latest/projectconf/section_env_build.html)
*/

#ifndef _VMA209_DISPLAY_H
#define _VMA209_DISPLAY_H

#include <stdio.h>

#define DISPLAY_LIB_SUCCES 0
#define DISPLAY_LIB_ERROR 1

#define NUMBER_OF_SEGMENTS 8

uint8_t initDisplay();

uint8_t dpClearSegment(uint8_t segment);
uint8_t dpClear();

uint8_t dpWriteNumberToSegment(uint8_t value, uint8_t segment);
uint8_t dpWriteNumber(uint16_t number);

uint8_t dpWriteCharToSegment(char character, uint8_t segment);
uint8_t dpWriteString(const char* string);

#if defined(__VMA209_LIB_DEBUG__)
    const char* dpGetLastError();
#endif

#endif
