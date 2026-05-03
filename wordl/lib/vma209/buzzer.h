/**
 * Buzzer library for Velleman VMA209 Arduino extension board
 * Author: Bauwen De Ron
 * Debug mode: in order to enable advanced debug mode, enable the __VMA209_LIB_DEBUG__ precompiler flag (for more information see: https://docs.platformio.org/en/latest/projectconf/section_env_build.html)
*/

#ifndef _VMA209_BUZZER_H
#define _VMA209_BUZZER_H

#include <stdio.h>

#define BUZZER_LIB_ERROR 0
#define BUZZER_LIB_SUCCES 1

uint8_t initBuzzer();

uint8_t bzPlayTone(float tone, uint32_t msDuration);

#if defined(__VMA209_LIB_DEBUG__)
  const char* bzGetLastFeedback();
#endif

#endif