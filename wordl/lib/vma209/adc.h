/**
 * ADC or Potentiometer library for Velleman VMA209 Arduino extension board
 * Author: Bauwen De Ron
 * Debug mode: in order to enable advanced debug mode, enable the __VMA209_LIB_DEBUG__ precompiler flag (for more information see: https://docs.platformio.org/en/latest/projectconf/section_env_build.html)
*/

#ifndef _VMA209_ADC_H
#define _VMA209_ADC_H

#include <stdio.h>

#define ADC_LIB_ERROR -1
#define ADC_LIB_SUCCES 1

uint8_t initADC();

int16_t adGetValue();

#if defined(__VMA209_LIB_DEBUG__)
  const char* adGetLastError();
#endif

#endif